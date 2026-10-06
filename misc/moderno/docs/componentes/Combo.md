# Combo

Menú desplegable para elegir una opción entre cinco o más (resolución, interpolación, idioma).

- Cerrado: 32 px de alto, fondo `bg-frame`, `radius-md`, chevron en `text-secondary`.
- Abierto: popup `bg-popup` con borde `border-subtle`, `radius-lg`; la opción actual lleva un check en `accent`.
- Las opciones dicen el resultado, no el valor técnico solo: «x2 · 60 Hz» mejor que «2».
- Con 2 a 4 opciones, prefiere SegmentedControl: se ven todas sin abrir nada.

## Código

```cpp
static const char* const filtros[] = { "Ninguno", "Bilineal", "Bicúbico", "xBRZ · 2x", "xBRZ · 4x", "ScaleForce" };
ImGuiModerno::SettingRow("Filtro de texturas", "Suaviza o redibuja las texturas al ampliarlas.");
ImGuiModerno::Combo("##filtro", &cfg.filtro, filtros, IM_ARRAYSIZE(filtros));
```

Muestra hasta 8 opciones sin scroll; la seleccionada lleva el check en `accent` y recibe el foco al abrir.

Firmas completas en `docs/api.md`.

## Medidas

| Token | Valor | Escala | Dónde |
|---|---|---|---|
| `combo-padding` | 0 12px | `space-3` | Relleno horizontal (alto fijo 32 px). |
| `combo-chevron-gap` | 8px | `space-2` | Mínimo entre el texto y el chevron. |
| `popup-offset` | 4px | `space-1` | Distancia entre el control y su lista desplegada. |
| `popup-padding` | 4px | `space-1` | Relleno interno de la lista. |
| `popup-item-padding` | 0 12px | `space-3` | Relleno de cada opción (alto 32 px). |
