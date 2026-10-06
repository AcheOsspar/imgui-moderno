#!/usr/bin/env python3
"""Comprueba que tokens.json e imgui_moderno.h dicen lo mismo.

Uso:  python misc/moderno/tools/check_tokens.py
Sale con código 1 y lista las diferencias si algo no coincide.
Revisa: colores de ambos modos, medidas de layout/espaciado/radios y los tamaños de componente que el header define.
"""
import json, re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
header = (ROOT / "imgui_moderno.h").read_text(encoding="utf-8")
tokens = json.loads((ROOT / "tokens.json").read_text(encoding="utf-8"))
errors = []

def camel(name):
    a, *rest = name.split("-")
    return a + "".join(w.capitalize() for w in rest)

def px(v):
    m = re.match(r"^\s*([\d.]+)", str(v))
    return float(m.group(1)) if m else None

# --- Colores: orden de los campos de Palette y valores de Dark()/Light() ---
fields = []
for line in re.search(r"struct Palette \{(.*?)\};", header, re.S).group(1).splitlines():
    line = line.strip().rstrip(";").replace("ImVec4", "")
    fields += [f.strip() for f in line.split(",") if f.strip()]

def palette(fn):
    body = re.search(fn + r"\(\) \{\s*static const Palette p = \{(.*?)\};", header, re.S).group(1)
    vals = re.findall(r"Hex\(0x([0-9a-fA-F]{6})(?:,\s*([\d.]+)f?)?\)", body)
    return {f: ("#" + h.lower(), float(a) if a else 1.0) for f, (h, a) in zip(fields, vals)}

pal = {"dark": palette("Dark"), "light": palette("Light")}
for tok in tokens["color"]["tokens"]:
    field = camel(tok["name"])
    if field not in fields:
        errors.append(f"color '{tok['name']}' no existe en Palette ({field})")
        continue
    for mode in ("dark", "light"):
        want = str(tok["value"][mode]).lower()
        have_hex, have_a = pal[mode][field]
        m = re.match(r"rgba\((\d+),\s*(\d+),\s*(\d+),\s*([\d.]+)\)", want)
        if m:
            want_hex = "#%02x%02x%02x" % tuple(int(m.group(i)) for i in (1, 2, 3))
            ok = want_hex == have_hex and abs(float(m.group(4)) - have_a) < 1e-3
        else:
            ok = want[:7] == have_hex
        if not ok:
            errors.append(f"color '{tok['name']}' ({mode}): tokens.json {want} ≠ header {have_hex} a={have_a}")

# --- Medidas: constantes de namespace M ---
consts = {k: float(v) for k, v in re.findall(r"\b(\w+)\s*=\s*([\d.]+)f?(?=[,;])", re.search(r"namespace M \{(.*?)\n\}", header, re.S).group(1))}
pairs = {
    # tokens.json → constante de M
    "space-1": "space1", "space-2": "space2", "space-3": "space3", "space-4": "space4", "space-5": "space5", "space-6": "space6",
    "radius-sm": "radiusSm", "radius-md": "radiusMd", "radius-lg": "radiusLg", "radius-xl": "radiusXl", "radius-full": "radiusFull",
    "window-width": "windowW", "window-height": "windowH", "sidebar-width": "sidebarW", "nav-item-height": "navItemH",
    "row-min-height": "rowMinH", "control-width": "controlW", "footer-height": "footerH", "icon-size": "iconSize",
    "control-height": "controlH", "segment-height": "segH", "badge-height": "badgeH", "modal-width": "modalW",
    "nav-item-gap": "navItemGap", "toggle-inset": "toggleInset", "slider-track": "sliderTrack", "slider-knob": "sliderKnob",
    "slider-value-width": "sliderValueW", "seg-padding": "segPad", "tooltip-max-width": "tooltipMaxW",
    "icon-stroke": "iconStroke",
}
all_tokens = {}
for sec in ("spacing", "radius", "layout", "medidas"):
    for tok in tokens[sec]["tokens"]:
        all_tokens[tok["name"]] = tok["value"]
for name, const in pairs.items():
    if name not in all_tokens:
        errors.append(f"falta el token '{name}' en tokens.json")
    elif const not in consts:
        errors.append(f"falta la constante M::{const} en el header")
    elif px(all_tokens[name]) != consts[const]:
        errors.append(f"'{name}' = {all_tokens[name]} en tokens.json, pero M::{const} = {consts[const]:g}")
motion = {x["name"]: x["value"] for x in tokens.get("motion", {}).get("tokens", [])}
if "motion-duration" not in motion:
    errors.append("falta el token 'motion-duration' en tokens.json")
elif px(motion["motion-duration"]) / 1000.0 != consts.get("motion"):
    errors.append(f"'motion-duration' = {motion['motion-duration']}, pero M::motion = {consts.get('motion')} s")

if "toggle-size" in all_tokens:
    w, h = re.findall(r"[\d.]+", all_tokens["toggle-size"])[:2]
    if (float(w), float(h)) != (consts.get("toggleW"), consts.get("toggleH")):
        errors.append(f"'toggle-size' = {all_tokens['toggle-size']}, pero M::toggleW/H = {consts.get('toggleW'):g}/{consts.get('toggleH'):g}")

# --- Tipografía ---
sizes = {"window-title": "fontWindowTitle", "page-title": "fontPageTitle", "section-label": "fontSection",
         "label": "fontLabel", "body": "fontBody", "caption": "fontCaption", "value": "fontValue"}
for group in tokens["type"]["groups"]:
    for st in group["styles"]:
        const = sizes.get(st["name"])
        if const and px(st["fontSize"]) != consts.get(const):
            errors.append(f"tipografía '{st['name']}' = {st['fontSize']}, pero M::{const} = {consts.get(const)}")

if errors:
    print("tokens.json e imgui_moderno.h NO coinciden:")
    for e in errors:
        print("  -", e)
    sys.exit(1)
print(f"OK: {len(tokens['color']['tokens'])} colores × 2 modos, {len(pairs)} medidas y {len(sizes)} estilos de texto coinciden.")
