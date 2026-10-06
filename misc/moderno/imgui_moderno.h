// imgui_moderno.h — Sistema de diseño "ImGui Moderno" (paleta Grafito) para Dear ImGui
// Header-only. Requiere Dear ImGui 1.92 o posterior (fuentes dinámicas: PushFont(font, size)).
// Copyright (c) 2026 AcheOsspar. Licencia MIT (ver misc/moderno/LICENSE).
// Hecho sobre Dear ImGui, Copyright (c) 2014-2026 Omar Cornut, licencia MIT.
//
// Uso mínimo:
//   #include "imgui_moderno.h"
//   ImGuiModerno::LoadFonts("Inter-Regular.ttf", "Inter-Medium.ttf", "Inter-SemiBold.ttf", "JetBrainsMono-Regular.ttf"); // opcional
//   ImGuiModerno::ApplyTheme(/*dark=*/true, /*dpi=*/main_scale);   // al iniciar y al cambiar modo o escala
//
//   if (ImGuiModerno::BeginSettingRows("##graficos")) {
//       ImGuiModerno::SettingRow("Contornos", "Bordes oscuros en personajes y objetos.");
//       ImGuiModerno::ToggleSwitch("##contornos", &cfg.outlines);
//       ImGuiModerno::EndSettingRows();
//   }
//
// Demo completa (ventana de ajustes de un emulador): ImGuiModerno::ShowDemoWindow() en imgui_moderno_demo.cpp.
// Los valores salen de tokens.json. Si cambias un token, cámbialo también aquí.
#pragma once

#include "imgui.h"
#include "imgui_internal.h"   // ImLerp, ImClamp, ImRect, RenderCheckMark, SetKeyOwner, ImFileOpen
#include <cfloat>
#include <cstdint>
#include <climits>
#include <cmath>
#include <initializer_list>

namespace ImGuiModerno {

// ---------------------------------------------------------------------------
// Color
// ---------------------------------------------------------------------------
struct Palette {
    ImVec4 bgWindow, bgSidebar, bgPopup, bgFrame, bgFrameHover, bgFrameActive, bgOverlay;
    ImVec4 textPrimary, textSecondary, textMuted, textDisabled;
    ImVec4 accent, accentHover, accentSoft, onAccent;
    ImVec4 borderSubtle, separator, knob, knobOff, scrollbarGrab;
    ImVec4 success, successSoft, warning, warningSoft, danger, dangerSoft;
};

inline ImVec4 Hex(uint32_t rgb, float a = 1.0f) {
    return ImVec4(((rgb >> 16) & 0xFF) / 255.0f, ((rgb >> 8) & 0xFF) / 255.0f, (rgb & 0xFF) / 255.0f, a);
}

inline const Palette& Dark() {
    static const Palette p = {
        Hex(0x141416), Hex(0x1a1a1d), Hex(0x1f1f22), Hex(0x27272a), Hex(0x2f2f33), Hex(0x3a3a3f), Hex(0x000000, 0.6f),
        Hex(0xededef), Hex(0xa1a1aa), Hex(0x8a8a93), Hex(0x55555c),
        Hex(0xededef), Hex(0xffffff), Hex(0x2a2a2e), Hex(0x18181b),
        Hex(0x2a2a2e), Hex(0x232326), Hex(0x18181b), Hex(0x8a8a93), Hex(0x3a3a3f),
        Hex(0x4cc38a), Hex(0x142a1e), Hex(0xf2b84b), Hex(0x2e2410), Hex(0xf2716b), Hex(0x351b1a),
    };
    return p;
}

inline const Palette& Light() {
    static const Palette p = {
        Hex(0xfafafa), Hex(0xf2f2f3), Hex(0xffffff), Hex(0xe8e8ea), Hex(0xdedee1), Hex(0xd2d2d6), Hex(0x141416, 0.4f),
        Hex(0x18181b), Hex(0x52525b), Hex(0x6b6b74), Hex(0xa1a1aa),
        Hex(0x18181b), Hex(0x3f3f46), Hex(0xe4e4e7), Hex(0xffffff),
        Hex(0xdcdce0), Hex(0xe7e7ea), Hex(0xffffff), Hex(0xffffff), Hex(0xd2d2d6),
        Hex(0x1a7048), Hex(0xe2f4ea), Hex(0x8f5a06), Hex(0xfbf0d9), Hex(0xb8322c), Hex(0xfbe5e4),
    };
    return p;
}

// Paleta y escala activas (las usan los widgets de abajo).
inline Palette& Col()    { static Palette p = Dark(); return p; }
inline float&   Dpi()    { static float d = 1.0f; return d; }
inline bool&    IsDark() { static bool d = true; return d; }

// ---------------------------------------------------------------------------
// Idioma (docs/api.md, sección Idiomas). Español, inglés y portugués (Brasil).
// Los textos propios del sistema (p. ej. «Cancelar» del modal) siguen el idioma activo;
// tu aplicación puede usar Tr() para los suyos.
// ---------------------------------------------------------------------------
enum Language { Language_Spanish, Language_English, Language_Portuguese, Language_COUNT };

inline Language& CurrentLanguage() { static Language l = Language_Spanish; return l; }
inline void SetLanguage(Language l) { CurrentLanguage() = (l >= 0 && l < Language_COUNT) ? l : Language_Spanish; }

// Devuelve el texto del idioma activo:  Tr("Aplicar", "Apply", "Aplicar")
inline const char* Tr(const char* es, const char* en, const char* pt) {
    switch (CurrentLanguage()) {
    case Language_English:    return en;
    case Language_Portuguese: return pt;
    default:                  return es;
    }
}

// Nombre de cada idioma escrito en ese idioma, para un selector («Español», «English», «Português»).
inline const char* LanguageName(Language l) {
    static const char* const names[Language_COUNT] = { "Español", "English", "Português" };
    return (l >= 0 && l < Language_COUNT) ? names[l] : names[0];
}

// Tono de estado para insignias y avisos.
enum Tone { Tone_Neutral, Tone_Success, Tone_Warning, Tone_Danger };

inline void ToneColors(Tone tone, ImVec4* bg, ImVec4* fg) {
    const Palette& c = Col();
    switch (tone) {
    case Tone_Success: *bg = c.successSoft; *fg = c.success; break;
    case Tone_Warning: *bg = c.warningSoft; *fg = c.warning; break;
    case Tone_Danger:  *bg = c.dangerSoft;  *fg = c.danger;  break;
    default:           *bg = c.bgFrame;     *fg = c.textSecondary; break;
    }
}

// ---------------------------------------------------------------------------
// Medidas (docs/medidas.md). Valores a escala 1; multiplicar por Dpi() al usar.
// ---------------------------------------------------------------------------
namespace M {
    constexpr float space1 = 4, space2 = 8, space3 = 12, space4 = 16, space5 = 24, space6 = 32;
    constexpr float badgeH = 20, segH = 28, segPad = 2, controlH = 32, navItemH = 36, navItemGap = 2, rowMinH = 44, footerH = 56;
    constexpr float sidebarW = 200, controlW = 240, windowW = 880, windowH = 600;
    constexpr float toggleW = 36, toggleH = 20, toggleInset = 3;
    constexpr float sliderTrack = 4, sliderKnob = 14, sliderValueW = 44;
    constexpr float modalW = 400, tooltipMaxW = 280, iconSize = 16, iconStroke = 1.75f;
    constexpr float motion = 0.12f;   // segundos: duración de toda animación (switch, selección, pastilla)
    constexpr float radiusSm = 4, radiusMd = 6, radiusLg = 8, radiusXl = 12, radiusFull = 999;
    // Tipografía (px)
    constexpr float fontWindowTitle = 20, fontPageTitle = 17, fontSection = 12, fontLabel = 15, fontBody = 15, fontCaption = 13, fontValue = 13, fontBadge = 12;
}

// ---------------------------------------------------------------------------
// Tipografía
// ---------------------------------------------------------------------------
// Fuentes cargadas con LoadFonts(). Cualquiera puede ser nullptr: se usa la fuente actual con el tamaño del token.
struct FontSet { ImFont* regular = nullptr; ImFont* medium = nullptr; ImFont* semibold = nullptr; ImFont* mono = nullptr; };
inline FontSet& Fonts() { static FontSet f; return f; }

enum TextStyle {
    TextStyle_Body,         // 15 px, regular
    TextStyle_Label,        // 15 px, medium: etiquetas de fila y botones
    TextStyle_Caption,      // 13 px, regular: descripciones
    TextStyle_Section,      // 12 px, semibold: títulos de sección
    TextStyle_PageTitle,    // 17 px, semibold
    TextStyle_WindowTitle,  // 20 px, semibold
    TextStyle_Value,        // 13 px, mono: valores y códigos
    TextStyle_Badge,        // 12 px, medium
};

inline void PushTextStyle(TextStyle t) {
    const FontSet& f = Fonts();
    ImFont* font = f.regular; float px = M::fontBody;
    switch (t) {
    case TextStyle_Body:        font = f.regular;  px = M::fontBody; break;
    case TextStyle_Label:       font = f.medium;   px = M::fontLabel; break;
    case TextStyle_Caption:     font = f.regular;  px = M::fontCaption; break;
    case TextStyle_Section:     font = f.semibold; px = M::fontSection; break;
    case TextStyle_PageTitle:   font = f.semibold; px = M::fontPageTitle; break;
    case TextStyle_WindowTitle: font = f.semibold; px = M::fontWindowTitle; break;
    case TextStyle_Value:       font = f.mono;     px = M::fontValue; break;
    case TextStyle_Badge:       font = f.medium;   px = M::fontBadge; break;
    }
    ImGui::PushFont(font, px);   // font == nullptr conserva la fuente actual
}
inline void PopTextStyle() { ImGui::PopFont(); }

namespace detail {
    inline bool FileExists(const char* path) {
        if (!path || !path[0]) return false;
        ImFileHandle f = ImFileOpen(path, "rb");
        if (!f) return false;
        ImFileClose(f);
        return true;
    }
    inline ImFont* TryAddFont(const char* path) {
        return FileExists(path) ? ImGui::GetIO().Fonts->AddFontFromFileTTF(path) : nullptr;
    }
}

// Carga Inter (regular, medium, semibold) y JetBrains Mono. Llamar antes del primer frame.
// Las rutas que falten se ignoran y esos estilos usan la fuente por defecto, así que siempre es seguro llamarla.
inline void LoadFonts(const char* regular, const char* medium, const char* semibold, const char* mono) {
    FontSet& f = Fonts();
    f.regular = detail::TryAddFont(regular);        // la primera fuente es la predeterminada
    if (!f.regular) f.regular = ImGui::GetIO().Fonts->AddFontDefaultVector();
    f.medium   = detail::TryAddFont(medium);
    f.semibold = detail::TryAddFont(semibold);
    f.mono     = detail::TryAddFont(mono);
    if (!f.medium)   f.medium = f.regular;
    if (!f.semibold) f.semibold = f.medium;
}

// ---------------------------------------------------------------------------
// Tema
// ---------------------------------------------------------------------------
// Llamar al iniciar y al cambiar modo o escala. Si se llama a mitad de frame, hazlo fuera de cualquier Push/Pop.
inline void ApplyTheme(bool dark, float dpi) {
    Col() = dark ? Dark() : Light();
    Dpi() = dpi;
    IsDark() = dark;
    const Palette& col = Col();

    ImGuiStyle& s = ImGui::GetStyle();
    s = ImGuiStyle();

    s.WindowPadding     = ImVec2(0, 0);
    s.FramePadding      = ImVec2(12, 6);
    s.ItemSpacing       = ImVec2(8, 8);
    s.ItemInnerSpacing  = ImVec2(8, 4);
    s.CellPadding       = ImVec2(0, 12);
    s.IndentSpacing     = 24;
    s.ScrollbarSize     = 10;
    s.GrabMinSize       = 14;

    s.WindowBorderSize  = 0;
    s.ChildBorderSize   = 0;
    s.PopupBorderSize   = 1;
    s.FrameBorderSize   = 0;
    s.TabBorderSize     = 0;
    s.SeparatorTextBorderSize = 1;

    s.WindowRounding    = M::radiusXl;
    s.ChildRounding     = M::radiusLg;
    s.PopupRounding     = M::radiusLg;
    s.FrameRounding     = M::radiusMd;
    s.TabRounding       = M::radiusMd;
    s.GrabRounding      = M::radiusFull;
    s.ScrollbarRounding = M::radiusFull;

    s.WindowTitleAlign    = ImVec2(0.0f, 0.5f);
    s.SelectableTextAlign = ImVec2(0.0f, 0.5f);
    s.HoverDelayShort     = 0.40f;   // tooltip: 400 ms
    s.ScaleAllSizes(dpi);

    s.FontSizeBase = M::fontBody;
    s._NextFrameFontSizeBase = M::fontBody;   // igual que imgui_demo.cpp al cambiar el tamaño a mitad de frame
    s.FontScaleDpi = dpi;

    ImVec4* c = s.Colors;
    const ImVec4 clear(0, 0, 0, 0);
    c[ImGuiCol_WindowBg]             = col.bgWindow;
    c[ImGuiCol_ChildBg]              = clear;
    c[ImGuiCol_PopupBg]              = col.bgPopup;
    c[ImGuiCol_Border]               = col.borderSubtle;
    c[ImGuiCol_BorderShadow]         = clear;
    c[ImGuiCol_Text]                 = col.textPrimary;
    c[ImGuiCol_TextDisabled]         = col.textDisabled;
    c[ImGuiCol_TextSelectedBg]       = col.accentSoft;
    c[ImGuiCol_FrameBg]              = col.bgFrame;
    c[ImGuiCol_FrameBgHovered]       = col.bgFrameHover;
    c[ImGuiCol_FrameBgActive]        = col.bgFrameActive;
    c[ImGuiCol_TitleBg]              = col.bgWindow;
    c[ImGuiCol_TitleBgActive]        = col.bgWindow;
    c[ImGuiCol_TitleBgCollapsed]     = col.bgWindow;
    c[ImGuiCol_MenuBarBg]            = col.bgSidebar;
    c[ImGuiCol_ScrollbarBg]          = clear;
    c[ImGuiCol_ScrollbarGrab]        = col.scrollbarGrab;
    c[ImGuiCol_ScrollbarGrabHovered] = col.bgFrameActive;
    c[ImGuiCol_ScrollbarGrabActive]  = col.textMuted;
    c[ImGuiCol_CheckMark]            = col.accent;
    c[ImGuiCol_SliderGrab]           = col.accent;
    c[ImGuiCol_SliderGrabActive]     = col.accentHover;
    c[ImGuiCol_Button]               = col.bgFrame;
    c[ImGuiCol_ButtonHovered]        = col.bgFrameHover;
    c[ImGuiCol_ButtonActive]         = col.bgFrameActive;
    c[ImGuiCol_Header]               = col.accentSoft;
    c[ImGuiCol_HeaderHovered]        = col.bgFrameHover;
    c[ImGuiCol_HeaderActive]         = col.accentSoft;
    c[ImGuiCol_Separator]            = col.separator;
    c[ImGuiCol_SeparatorHovered]     = col.accent;
    c[ImGuiCol_SeparatorActive]      = col.accent;
    c[ImGuiCol_ResizeGrip]           = clear;
    c[ImGuiCol_ResizeGripHovered]    = col.bgFrameActive;
    c[ImGuiCol_ResizeGripActive]     = col.accent;
    c[ImGuiCol_Tab]                  = col.bgSidebar;
    c[ImGuiCol_TabHovered]           = col.bgFrameHover;
    c[ImGuiCol_TabSelected]          = col.bgFrame;
    c[ImGuiCol_TabSelectedOverline]  = col.accent;
    c[ImGuiCol_TableHeaderBg]        = col.bgSidebar;
    c[ImGuiCol_TableBorderStrong]    = col.separator;
    c[ImGuiCol_TableBorderLight]     = col.separator;
    c[ImGuiCol_TableRowBg]           = clear;
    c[ImGuiCol_TableRowBgAlt]        = clear;
    c[ImGuiCol_NavCursor]            = col.accent;
    c[ImGuiCol_ModalWindowDimBg]     = col.bgOverlay;
    c[ImGuiCol_NavWindowingDimBg]    = col.bgOverlay;
    c[ImGuiCol_DragDropTarget]       = col.accent;
}

// ---------------------------------------------------------------------------
// Utilidades internas
// ---------------------------------------------------------------------------
namespace detail {
    inline ImU32 U32(const ImVec4& c) { return ImGui::GetColorU32(c); }   // aplica style.Alpha (BeginDisabled)
    inline ImVec2 Add(ImVec2 a, ImVec2 b) { return ImVec2(a.x + b.x, a.y + b.y); }

    // Ancho de la caja del control: el de SetNextItemWidth() si se llamó, si no el ancho natural del widget.
    inline bool HasNextItemWidth() { return (GImGui->NextItemData.HasFlags & ImGuiNextItemDataFlags_HasWidth) != 0; }

    // Dentro de la columna de control (SetNextItemWidth activo), coloca un elemento de tamaño 'size'
    // pegado al borde derecho y centrado en el alto de un control. Fuera de ella no hace nada.
    inline void AlignInControlBox(ImVec2 size) {
        if (!HasNextItemWidth()) return;
        const float boxW = ImGui::CalcItemWidth();
        const float boxH = M::controlH * Dpi();
        if (boxW > size.x) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + boxW - size.x);
        if (boxH > size.y) ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (boxH - size.y) * 0.5f);
        GImGui->NextItemData.ClearFlags();   // ya se usó el ancho
    }

    // Acerca 'cur' a 'target' con la duración de movimiento del sistema (~95 % en M::motion).
    inline float Approach(float cur, float target, float snap) {
        const float k = 1.0f - std::exp(-ImGui::GetIO().DeltaTime * 3.0f / M::motion);
        const float v = cur + (target - cur) * k;
        return (std::fabs(target - v) < snap) ? target : v;
    }

    // Clave de almacenamiento fija por ventana (no depende de PushID del usuario).
    inline ImGuiID WindowKey(const char* name) { return ImHashStr(name, 0, ImGui::GetCurrentWindow()->ID); }

    // Texto con la fuente actual, sin crear un ítem.
    inline void DrawText(ImVec2 pos, const ImVec4& col, const char* text, const char* end = nullptr, float wrap = 0.0f) {
        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), pos, U32(col), text, end, wrap);
    }
}

// ---------------------------------------------------------------------------
// Toggle (docs/componentes/Toggle.md). Reemplaza a ImGui::Checkbox para opciones sí/no.
// Ocupa el alto de un control (32 px). Con SetNextItemWidth() se alinea a la derecha de esa caja,
// que es lo que hace SettingRow() para que todos los controles terminen en el mismo borde.
// ---------------------------------------------------------------------------
inline bool ToggleSwitch(const char* id, bool* v) {
    const Palette& col = Col();
    const float dpi = Dpi();
    const float w = M::toggleW * dpi, h = M::toggleH * dpi, r = h * 0.5f, boxH = M::controlH * dpi;

    if (detail::HasNextItemWidth()) {
        const float boxW = ImGui::CalcItemWidth();
        if (boxW > w) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + boxW - w);
    }
    const ImVec2 p = ImGui::GetCursorScreenPos();
    const bool pressed = ImGui::InvisibleButton(id, ImVec2(w, boxH));
    if (pressed) *v = !*v;

    ImGuiStorage* st = ImGui::GetStateStorage();
    const ImGuiID key = ImGui::GetItemID();
    float t = st->GetFloat(key, *v ? 1.0f : 0.0f);
    t = ImClamp(t + (*v ? 1.0f : -1.0f) * ImGui::GetIO().DeltaTime / M::motion, 0.0f, 1.0f);
    st->SetFloat(key, t);

    const float y = p.y + (boxH - h) * 0.5f;
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec4 track = ImLerp(col.bgFrameActive, col.accent, t);
    if (ImGui::IsItemHovered() && t < 0.5f) track = ImLerp(track, col.textMuted, 0.25f);
    dl->AddRectFilled(ImVec2(p.x, y), ImVec2(p.x + w, y + h), detail::U32(track), r);
    const ImVec4 knob = ImLerp(col.knobOff, col.knob, t);
    const float kr = r - M::toggleInset * dpi;
    dl->AddCircleFilled(ImVec2(p.x + r + t * (w - h), y + r), kr, detail::U32(knob));
    return pressed;
}

// ---------------------------------------------------------------------------
// Selector segmentado (docs/componentes/SegmentedControl.md). De 2 a 4 opciones.
// *v = -1 deja todos los segmentos sin marcar (estado «Personalizado»).
// ---------------------------------------------------------------------------
inline bool Segmented(const char* id, int* v, const char* const items[], int count) {
    const Palette& col = Col();
    const float dpi = Dpi();
    const float width = ImGui::CalcItemWidth();
    const float H = M::controlH * dpi, pad = M::segPad * dpi;
    const ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + width, p.y + H), detail::U32(col.bgFrame), M::radiusMd * dpi);

    bool changed = false;
    const float segW = (width - 2 * pad) / (float)count;
    const ImVec2 segSize(segW, H - 2 * pad);
    ImGui::PushID(id);

    // Pastilla del segmento activo: se desliza entre opciones y se desvanece en «Personalizado» (*v = -1).
    ImGuiStorage* st = ImGui::GetStateStorage();
    const ImGuiID keyX = ImGui::GetID("##pastillaX"), keyA = ImGui::GetID("##pastillaA");
    const bool hasSel = (*v >= 0 && *v < count);
    const float targetX = hasSel ? segW * (float)*v : st->GetFloat(keyX, 0.0f);
    const float pillX = detail::Approach(st->GetFloat(keyX, targetX), targetX, 0.25f);
    const float pillA = detail::Approach(st->GetFloat(keyA, hasSel ? 1.0f : 0.0f), hasSel ? 1.0f : 0.0f, 0.01f);
    st->SetFloat(keyX, pillX);
    st->SetFloat(keyA, pillA);
    if (pillA > 0.0f) {
        ImVec4 c = col.accent; c.w *= pillA;
        const ImVec2 a(p.x + pad + pillX, p.y + pad);
        dl->AddRectFilled(a, ImVec2(a.x + segSize.x, a.y + segSize.y), detail::U32(c), M::radiusSm * dpi);
    }

    PushTextStyle(TextStyle_Label);
    for (int i = 0; i < count; i++) {
        const ImVec2 a(p.x + pad + segW * i, p.y + pad);
        ImGui::SetCursorScreenPos(a);
        ImGui::PushID(i);
        if (ImGui::InvisibleButton("##seg", segSize) && *v != i) { *v = i; changed = true; }
        ImGui::PopID();
        const bool hovered = ImGui::IsItemHovered();
        const ImVec2 b(a.x + segSize.x, a.y + segSize.y);
        // Cuánto cubre la pastilla a este segmento (0 a 1): decide si se dibuja el fondo de hover.
        const float cover = pillA * ImClamp(1.0f - std::fabs(pillX - segW * i) / segW, 0.0f, 1.0f);
        if (hovered && cover < 0.05f)
            dl->AddRectFilled(a, b, detail::U32(col.bgFrameHover), M::radiusSm * dpi);
        // Texto en dos pasadas: color normal fuera de la pastilla y on-accent recortado a la pastilla,
        // así se lee bien también mientras se desliza.
        const ImVec2 ts = ImGui::CalcTextSize(items[i]);
        const ImVec2 t0(a.x + M::space2 * dpi, a.y), t1(b.x - M::space2 * dpi, b.y);
        const ImRect bb(a, b);
        const ImRect pill(p.x + pad + pillX, a.y, p.x + pad + pillX + segW, b.y);
        ImRect inPill = bb; inPill.ClipWith(pill);
        const bool pillHere = pillA > 0.0f && inPill.GetWidth() > 0.0f;
        ImRect outPill = bb;   // la parte del segmento que la pastilla no cubre (a un lado u otro)
        if (pillHere && pillA >= 0.99f) {
            if (pill.Min.x <= bb.Min.x) outPill.Min.x = ImMin(bb.Max.x, pill.Max.x);
            else                        outPill.Max.x = ImMax(bb.Min.x, pill.Min.x);
        }
        if (outPill.GetWidth() > 0.0f) {
            ImGui::PushStyleColor(ImGuiCol_Text, hovered ? col.textPrimary : col.textSecondary);
            ImGui::RenderTextClipped(t0, t1, items[i], nullptr, &ts, ImVec2(0.5f, 0.5f), &outPill);
            ImGui::PopStyleColor();
        }
        if (pillHere) {
            ImVec4 on = ImLerp(col.textSecondary, col.onAccent, pillA);
            ImGui::PushStyleColor(ImGuiCol_Text, on);
            ImGui::RenderTextClipped(t0, t1, items[i], nullptr, &ts, ImVec2(0.5f, 0.5f), &inPill);
            ImGui::PopStyleColor();
        }
    }
    PopTextStyle();
    ImGui::PopID();
    ImGui::SetCursorScreenPos(p);
    ImGui::Dummy(ImVec2(width, H));
    return changed;
}

// ---------------------------------------------------------------------------
// Íconos de línea dibujados con ImDrawList (estilo Lucide, rejilla de 24, trazo de 1.75 px a 16 px).
// No necesitan ninguna fuente de íconos. Se usan en SidebarItem y en los botones.
// ---------------------------------------------------------------------------
enum Icon {
    Icon_None,
    Icon_Monitor,        // Video
    Icon_Palette,        // Gráficos
    Icon_Camera,         // Cámara
    Icon_Volume,         // Audio
    Icon_Gamepad,        // Controles
    Icon_Accessibility,  // Accesibilidad
    Icon_Info,           // Acerca de
    Icon_Help,           // Ayuda
    Icon_Folder,         // Abrir archivo
    Icon_Refresh,        // Restablecer, requiere reinicio
    Icon_Trash,          // Borrar
    Icon_COUNT
};

// Dibuja 'icon' en un cuadro de 'size' px cuya esquina superior izquierda es 'pos'.
inline void DrawIcon(ImDrawList* dl, Icon icon, ImVec2 pos, float size, ImU32 col) {
    const float s = size / 24.0f;
    const float th = M::iconStroke * size / M::iconSize;
    auto P = [&](float x, float y) { return ImVec2(pos.x + x * s, pos.y + y * s); };
    auto line = [&](float x0, float y0, float x1, float y1) { dl->AddLine(P(x0, y0), P(x1, y1), col, th); };
    auto dot = [&](float x, float y, float r) { dl->AddCircleFilled(P(x, y), r * s, col); };
    auto poly = [&](std::initializer_list<ImVec2> pts, bool closed) {
        ImVec2 buf[16]; int n = 0;
        for (const ImVec2& q : pts) buf[n++] = P(q.x, q.y);
        dl->AddPolyline(buf, n, col, closed ? ImDrawFlags_Closed : ImDrawFlags_None, th);
    };
    auto arc = [&](float cx, float cy, float r, float a0, float a1) {
        dl->PathArcTo(P(cx, cy), r * s, a0, a1, 0);
        dl->PathStroke(col, ImDrawFlags_None, th);
    };
    const float PI = 3.14159265f;
    switch (icon) {
    case Icon_Monitor:
        dl->AddRect(P(2, 3), P(22, 17), col, 2 * s, ImDrawFlags_None, th);
        line(8, 21, 16, 21); line(12, 17, 12, 21);
        break;
    case Icon_Palette:
        dl->AddCircle(P(12, 12), 10 * s, col, 0, th);
        dot(13.5f, 6.5f, 1.6f); dot(17.5f, 10.5f, 1.6f); dot(8.5f, 7.5f, 1.6f); dot(6.5f, 12.5f, 1.6f);
        break;
    case Icon_Camera:
        dl->AddRect(P(2, 7), P(22, 20), col, 2 * s, ImDrawFlags_None, th);
        poly({ ImVec2(7.5f, 7), ImVec2(9, 4), ImVec2(15, 4), ImVec2(16.5f, 7) }, false);
        dl->AddCircle(P(12, 13.5f), 3.5f * s, col, 0, th);
        break;
    case Icon_Volume:
        poly({ ImVec2(11, 5), ImVec2(6, 9), ImVec2(2, 9), ImVec2(2, 15), ImVec2(6, 15), ImVec2(11, 19) }, true);
        arc(12, 12, 5, -PI * 0.25f, PI * 0.25f);
        arc(12, 12, 10, -PI * 0.25f, PI * 0.25f);
        break;
    case Icon_Gamepad:
        dl->AddRect(P(2, 6), P(22, 18), col, 6 * s, ImDrawFlags_None, th);
        line(6, 12, 10, 12); line(8, 10, 8, 14);
        dot(15, 13, 1.3f); dot(18, 10.5f, 1.3f);
        break;
    case Icon_Accessibility:
        dl->AddCircle(P(12, 12), 10 * s, col, 0, th);
        dot(12, 6.8f, 1.6f);
        line(7.5f, 10, 16.5f, 10); line(12, 10, 12, 14);
        poly({ ImVec2(9, 18), ImVec2(12, 14), ImVec2(15, 18) }, false);
        break;
    case Icon_Info:
        dl->AddCircle(P(12, 12), 10 * s, col, 0, th);
        line(12, 16, 12, 11.5f); dot(12, 8, 1.3f);
        break;
    case Icon_Help:
        dl->AddCircle(P(12, 12), 10 * s, col, 0, th);
        dl->PathArcTo(P(12, 9.5f), 2.75f * s, PI, PI * 2.3f, 0);
        dl->PathLineTo(P(12, 13.5f));
        dl->PathStroke(col, ImDrawFlags_None, th);
        dot(12, 17, 1.3f);
        break;
    case Icon_Folder:
        poly({ ImVec2(2, 5), ImVec2(9, 5), ImVec2(11, 7.5f), ImVec2(22, 7.5f), ImVec2(22, 20), ImVec2(2, 20) }, true);
        break;
    case Icon_Refresh:
        arc(12, 12, 9, PI * 1.11f, PI * 1.83f);
        poly({ ImVec2(21, 3), ImVec2(21, 8), ImVec2(16, 8) }, false);
        arc(12, 12, 9, PI * 0.11f, PI * 0.83f);
        poly({ ImVec2(3, 21), ImVec2(3, 16), ImVec2(8, 16) }, false);
        break;
    case Icon_Trash:
        line(3, 6, 21, 6);
        poly({ ImVec2(5, 6), ImVec2(6, 21), ImVec2(18, 21), ImVec2(19, 6) }, false);
        poly({ ImVec2(9, 6), ImVec2(9, 3), ImVec2(15, 3), ImVec2(15, 6) }, false);
        break;
    default: break;
    }
}

// ---------------------------------------------------------------------------
// Botones (docs/componentes/Button.md). Alto fijo de 32 px, relleno horizontal de 16 px.
// Un solo PrimaryButton por vista; DangerButton siempre con ConfirmModal.
// ---------------------------------------------------------------------------
enum ButtonKind { ButtonKind_Primary, ButtonKind_Secondary, ButtonKind_Ghost, ButtonKind_Danger };

inline float ButtonWidth(const char* label, Icon icon = Icon_None);

// 'icon' (opcional) va a la izquierda del texto, a 8 px.
inline bool ButtonEx(const char* label, ButtonKind kind, float width = 0.0f, Icon icon = Icon_None) {
    const Palette& col = Col();
    const float dpi = Dpi();
    ImVec4 bg, hover, active, text;
    switch (kind) {
    case ButtonKind_Primary:   bg = col.accent;     hover = col.accentHover;  active = col.accentHover;   text = col.onAccent; break;
    case ButtonKind_Ghost:     bg = ImVec4(0, 0, 0, 0); hover = col.bgFrameHover; active = col.bgFrameActive; text = col.textSecondary; break;
    case ButtonKind_Danger:    bg = col.dangerSoft; hover = ImLerp(col.dangerSoft, col.danger, 0.12f); active = ImLerp(col.dangerSoft, col.danger, 0.22f); text = col.danger; break;
    default:                   bg = col.bgFrame;    hover = col.bgFrameHover; active = col.bgFrameActive; text = col.textPrimary; break;
    }
    ImGui::PushStyleColor(ImGuiCol_Button, bg);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hover);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, active);
    ImGui::PushStyleColor(ImGuiCol_Text, text);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(M::space4 * dpi, 0));
    PushTextStyle(TextStyle_Label);
    const float H = M::controlH * dpi;
    if (width <= 0.0f && detail::HasNextItemWidth())
        detail::AlignInControlBox(ImVec2(ButtonWidth(label, icon), H));
    bool pressed;
    if (icon == Icon_None) {
        pressed = ImGui::Button(label, ImVec2(width, H));
    } else {
        // Botón sin texto propio; ícono y etiqueta se dibujan centrados encima.
        const float w = width > 0.0f ? width : ButtonWidth(label, icon);
        ImGui::PushID(label);
        pressed = ImGui::Button("##boton", ImVec2(w, H));
        ImGui::PopID();
        const ImVec2 mn = ImGui::GetItemRectMin();
        const float iconSz = M::iconSize * dpi, gap = M::space2 * dpi;
        const float textW = ImGui::CalcTextSize(label, nullptr, true).x;
        const float x = mn.x + (w - (iconSz + gap + textW)) * 0.5f;
        DrawIcon(ImGui::GetWindowDrawList(), icon, ImVec2(x, mn.y + (H - iconSz) * 0.5f), iconSz, detail::U32(text));
        detail::DrawText(ImVec2(x + iconSz + gap, mn.y + (H - ImGui::GetFontSize()) * 0.5f), text, label, ImGui::FindRenderedTextEnd(label));
    }
    PopTextStyle();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(4);
    return pressed;
}
inline bool PrimaryButton(const char* label, Icon icon = Icon_None)   { return ButtonEx(label, ButtonKind_Primary, 0.0f, icon); }
inline bool SecondaryButton(const char* label, Icon icon = Icon_None) { return ButtonEx(label, ButtonKind_Secondary, 0.0f, icon); }
inline bool GhostButton(const char* label, Icon icon = Icon_None)     { return ButtonEx(label, ButtonKind_Ghost, 0.0f, icon); }
inline bool DangerButton(const char* label, Icon icon = Icon_None)    { return ButtonEx(label, ButtonKind_Danger, 0.0f, icon); }

// Ancho que ocupará un botón; útil para alinear grupos a la derecha con SameLineRight().
inline float ButtonWidth(const char* label, Icon icon) {
    PushTextStyle(TextStyle_Label);
    float w = ImGui::CalcTextSize(label, nullptr, true).x + 2 * M::space4 * Dpi();
    if (icon != Icon_None) w += (M::iconSize + M::space2) * Dpi();
    PopTextStyle();
    return w;
}

// Coloca el siguiente ítem en la misma línea, pegado al borde derecho, dejando 'width' de espacio.
inline void SameLineRight(float width) {
    const float x = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - width;
    ImGui::SameLine(x);
}

// ---------------------------------------------------------------------------
// Insignia (docs/componentes/Badge.md). 20 px de alto, una por fila como máximo.
// ---------------------------------------------------------------------------
inline ImVec2 BadgeSize(const char* text) {
    PushTextStyle(TextStyle_Badge);
    const ImVec2 ts = ImGui::CalcTextSize(text);
    PopTextStyle();
    return ImVec2(ts.x + 2 * M::space2 * Dpi(), M::badgeH * Dpi());
}

inline void RenderBadge(ImVec2 p, const char* text, const ImVec4& bg, const ImVec4& fg) {
    const float dpi = Dpi();
    const ImVec2 size = BadgeSize(text);
    ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), detail::U32(bg), M::radiusSm * dpi);
    PushTextStyle(TextStyle_Badge);
    detail::DrawText(ImVec2(p.x + M::space2 * dpi, p.y + (size.y - ImGui::GetFontSize()) * 0.5f), fg, text);
    PopTextStyle();
}

inline void Badge(const char* text, const ImVec4& bg, const ImVec4& fg) {
    detail::AlignInControlBox(BadgeSize(text));
    const ImVec2 p = ImGui::GetCursorScreenPos();
    RenderBadge(p, text, bg, fg);
    ImGui::Dummy(BadgeSize(text));
}
inline void Badge(const char* text, Tone tone = Tone_Neutral) {
    ImVec4 bg, fg; ToneColors(tone, &bg, &fg);
    Badge(text, bg, fg);
}

// ---------------------------------------------------------------------------
// Lectura de un valor en mono 13 px y text-secondary («v2.4.1», «90 Hz»). En la columna de control
// se alinea a la derecha y se centra como un control.
// ---------------------------------------------------------------------------
inline void Value(const char* text) {
    PushTextStyle(TextStyle_Value);
    detail::AlignInControlBox(ImGui::CalcTextSize(text));
    ImGui::PushStyleColor(ImGuiCol_Text, Col().textSecondary);
    ImGui::TextUnformatted(text);
    ImGui::PopStyleColor();
    PopTextStyle();
}

// ---------------------------------------------------------------------------
// Tooltip (docs/componentes/Tooltip.md). Aparece a los 400 ms; máximo 280 px de ancho.
// ---------------------------------------------------------------------------
inline bool BeginItemTooltip() {
    const float dpi = Dpi();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(M::space3 * dpi, M::space2 * dpi));
    const bool open = ImGui::BeginItemTooltip();
    ImGui::PopStyleVar();
    if (open) ImGui::PushTextWrapPos((M::tooltipMaxW - M::space3) * dpi);
    return open;
}
inline void EndItemTooltip() { ImGui::PopTextWrapPos(); ImGui::EndTooltip(); }

// Tooltip con título opcional y cuerpo para el último ítem.
inline void ItemTooltip(const char* title, const char* body) {
    if (!BeginItemTooltip()) return;
    if (title) {
        PushTextStyle(TextStyle_Label);
        ImGui::TextUnformatted(title);
        PopTextStyle();
        if (body) ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y + M::space1 * Dpi());
    }
    if (body) {
        PushTextStyle(TextStyle_Caption);
        ImGui::PushStyleColor(ImGuiCol_Text, Col().textSecondary);
        ImGui::TextUnformatted(body);
        ImGui::PopStyleColor();
        PopTextStyle();
    }
    EndItemTooltip();
}

// Ícono de ayuda «?» con tooltip. Enfocable con el mando para que el tooltip también aparezca.
inline void HelpMarker(const char* title, const char* body) {
    const Palette& col = Col();
    const float sz = ImGui::GetFontSize();
    const ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::PushID(body ? body : title);
    ImGui::InvisibleButton("##ayuda", ImVec2(sz, sz));
    ImGui::PopID();
    const bool hovered = ImGui::IsItemHovered() || ImGui::IsItemFocused();
    const ImVec4& c = hovered ? col.textPrimary : col.textMuted;
    ImDrawList* dl = ImGui::GetWindowDrawList();
    const ImVec2 center(p.x + sz * 0.5f, p.y + sz * 0.5f);
    dl->AddCircle(center, sz * 0.45f, detail::U32(c), 0, ImMax(1.0f, Dpi()));
    PushTextStyle(TextStyle_Badge);
    const ImVec2 ts = ImGui::CalcTextSize("?");
    detail::DrawText(ImVec2(center.x - ts.x * 0.5f, center.y - ts.y * 0.5f), c, "?");
    PopTextStyle();
    ItemTooltip(title, body);
}

// ---------------------------------------------------------------------------
// Título de sección (docs/componentes/SectionHeader.md). 24 px arriba (salvo la primera), 4 px abajo.
// ---------------------------------------------------------------------------
inline void SectionHeader(const char* text, bool first = false) {
    const float dpi = Dpi();
    const float spacing = ImGui::GetStyle().ItemSpacing.y;
    if (!first) ImGui::Dummy(ImVec2(0, ImMax(0.0f, M::space5 * dpi - 2 * spacing)));
    PushTextStyle(TextStyle_Section);
    ImGui::TextColored(Col().textMuted, "%s", text);
    PopTextStyle();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - spacing + M::space1 * dpi);
}

// Sección colapsable con chevron, para «Avanzado» (siempre la última de la categoría y cerrada por defecto).
// Devuelve true si está abierta; el estado se guarda por ventana.
//   if (CollapsibleSection("Avanzado") && BeginSettingRows("##avanzado")) { ...; EndSettingRows(); }
inline bool CollapsibleSection(const char* text, bool defaultOpen = false, bool first = false) {
    const Palette& col = Col();
    const float dpi = Dpi();
    const float spacing = ImGui::GetStyle().ItemSpacing.y;
    if (!first) ImGui::Dummy(ImVec2(0, ImMax(0.0f, M::space5 * dpi - 2 * spacing)));

    ImGuiStorage* st = ImGui::GetStateStorage();
    const ImGuiID key = ImGui::GetID(text);
    bool open = st->GetBool(key, defaultOpen);

    PushTextStyle(TextStyle_Section);
    const float h = ImGui::GetFontSize();
    const ImVec2 ts = ImGui::CalcTextSize(text, nullptr, true);
    const float chev = h * 0.3f;
    const ImVec2 p = ImGui::GetCursorScreenPos();
    if (ImGui::InvisibleButton(text, ImVec2(ts.x + M::space2 * dpi + chev * 2, h))) { open = !open; st->SetBool(key, open); }
    const ImVec4& c = (ImGui::IsItemHovered() || ImGui::IsItemFocused()) ? col.textSecondary : col.textMuted;
    detail::DrawText(p, c, text, ImGui::FindRenderedTextEnd(text));
    const ImVec2 cc(p.x + ts.x + M::space2 * dpi + chev, p.y + h * 0.5f);
    ImVec2 pts[3];
    if (open) { pts[0] = ImVec2(cc.x - chev, cc.y - chev * 0.5f); pts[1] = ImVec2(cc.x, cc.y + chev * 0.5f); pts[2] = ImVec2(cc.x + chev, cc.y - chev * 0.5f); }
    else      { pts[0] = ImVec2(cc.x - chev * 0.5f, cc.y - chev); pts[1] = ImVec2(cc.x + chev * 0.5f, cc.y); pts[2] = ImVec2(cc.x - chev * 0.5f, cc.y + chev); }
    ImGui::GetWindowDrawList()->AddPolyline(pts, 3, detail::U32(c), ImDrawFlags_None, ImMax(1.0f, 1.5f * dpi));
    PopTextStyle();
    // Solo si vienen filas debajo: mover el cursor tras el último ítem de una ventana es un error en ImGui.
    if (open) ImGui::SetCursorPosY(ImGui::GetCursorPosY() - spacing + M::space1 * dpi);
    return open;
}

// ---------------------------------------------------------------------------
// Fila de ajuste (docs/componentes/SettingRow.md). Llamar entre BeginSettingRows/EndSettingRows.
// Escribe etiqueta y descripción y deja el cursor en la columna del control (240 px, alineada a la derecha),
// centrado verticalmente con el texto. El control siguiente ocupa toda la columna.
// Para una opción deshabilitada envuelve la fila en ImGui::BeginDisabled() y explica el motivo en 'desc'.
// ---------------------------------------------------------------------------
struct RowOptions {
    const char* badge = nullptr;            // «Requiere reinicio», «Exigente», «Beta»…
    Tone        badgeTone = Tone_Warning;
    const char* helpTitle = nullptr;        // ícono «?» con tooltip junto a la etiqueta
    const char* helpBody = nullptr;
    bool        dependent = false;          // opción dependiente: sangría de 24 px bajo su opción madre
};

inline bool BeginSettingRows(const char* id) {
    if (!ImGui::BeginTable(id, 2, ImGuiTableFlags_BordersInnerH)) return false;
    ImGui::TableSetupColumn("texto", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("control", ImGuiTableColumnFlags_WidthFixed, M::controlW * Dpi());
    return true;
}
inline void EndSettingRows() { ImGui::EndTable(); }

inline void SettingRow(const char* label, const char* desc = nullptr, const RowOptions& opt = RowOptions()) {
    const Palette& col = Col();
    const float dpi = Dpi();
    const ImGuiStyle& style = ImGui::GetStyle();

    ImGui::TableNextRow(ImGuiTableRowFlags_None, M::rowMinH * dpi);
    ImGui::TableNextColumn();
    const float indent = opt.dependent ? M::space5 * dpi : 0.0f;
    if (indent > 0) ImGui::Indent(indent);

    // Medir el bloque de texto para centrarlo con el control.
    const float startY = ImGui::GetCursorPosY();
    const float wrapW = ImMax(1.0f, ImGui::GetContentRegionAvail().x - M::space5 * dpi);
    PushTextStyle(TextStyle_Label);
    const float labelH = ImGui::GetFontSize();
    PopTextStyle();
    float descH = 0.0f;
    if (desc) {
        PushTextStyle(TextStyle_Caption);
        descH = ImGui::CalcTextSize(desc, nullptr, false, wrapW).y;
        PopTextStyle();
    }
    const float textH = labelH + (desc ? M::space1 * dpi + descH : 0.0f);
    const float ctrlH = M::controlH * dpi;
    const float blockH = ImMax(textH, ctrlH);

    ImGui::SetCursorPosY(startY + (blockH - textH) * 0.5f);
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + wrapW);
    PushTextStyle(TextStyle_Label);
    ImGui::TextUnformatted(label);
    const ImVec2 labelMin = ImGui::GetItemRectMin(), labelMax = ImGui::GetItemRectMax();
    float extraX = 0.0f;
    if (opt.badge) {
        ImVec4 bg, fg; ToneColors(opt.badgeTone, &bg, &fg);
        const ImVec2 bs = BadgeSize(opt.badge);
        RenderBadge(ImVec2(labelMax.x + M::space2 * dpi, labelMin.y + (labelMax.y - labelMin.y - bs.y) * 0.5f), opt.badge, bg, fg);
        extraX = bs.x + M::space2 * dpi;
    }
    if (opt.helpTitle || opt.helpBody) {
        ImGui::SameLine(0, M::space2 * dpi + extraX);
        HelpMarker(opt.helpTitle, opt.helpBody);
    }
    PopTextStyle();
    if (desc) {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - style.ItemSpacing.y + M::space1 * dpi);
        PushTextStyle(TextStyle_Caption);
        ImGui::PushStyleColor(ImGuiCol_Text, col.textSecondary);
        ImGui::TextUnformatted(desc);
        ImGui::PopStyleColor();
        PopTextStyle();
    }
    ImGui::PopTextWrapPos();
    if (indent > 0) ImGui::Unindent(indent);

    ImGui::TableNextColumn();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (blockH - ctrlH) * 0.5f);
    ImGui::SetNextItemWidth(-FLT_MIN);
}

// ---------------------------------------------------------------------------
// Slider fino (docs/componentes/Slider.md). Pista de 4 px, perilla de 14 px y lectura del valor a la derecha.
// Clic derecho o doble clic vuelve a 'resetValue' (si se indica). Con teclado o mando: izquierda/derecha.
// ---------------------------------------------------------------------------
inline bool SliderFloat(const char* id, float* v, float vMin, float vMax, const char* format = "%.2f", float resetValue = FLT_MAX, float step = 0.0f) {
    const Palette& col = Col();
    const float dpi = Dpi();
    ImGuiContext& g = *GImGui;
    const float fullW = ImGui::CalcItemWidth();
    const float H = M::controlH * dpi;
    if (step <= 0.0f) step = (vMax - vMin) / 100.0f;

    char buf[64];
    ImFormatString(buf, IM_ARRAYSIZE(buf), format, *v);
    PushTextStyle(TextStyle_Value);
    const ImVec2 ts = ImGui::CalcTextSize(buf);
    PopTextStyle();
    const float valW = ImMax(ts.x, M::sliderValueW * dpi);
    const float trackW = ImMax(H, fullW - M::space3 * dpi - valW);

    const ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::PushID(id);
    ImGui::InvisibleButton("##slider", ImVec2(trackW, H));
    const ImGuiID itemId = ImGui::GetItemID();
    ImGui::PopID();
    const bool hovered = ImGui::IsItemHovered(), active = ImGui::IsItemActive(), focused = ImGui::IsItemFocused();

    const float knobR = M::sliderKnob * dpi * 0.5f;
    const float x0 = p.x + knobR, x1 = p.x + trackW - knobR;
    const float old = *v;
    if (active && g.ActiveIdSource == ImGuiInputSource_Mouse && x1 > x0) {
        const float t = ImClamp((g.IO.MousePos.x - x0) / (x1 - x0), 0.0f, 1.0f);
        *v = vMin + t * (vMax - vMin);
    }
    if (focused && !active) {
        const ImGuiKey keys[4] = { ImGuiKey_LeftArrow, ImGuiKey_RightArrow, ImGuiKey_GamepadDpadLeft, ImGuiKey_GamepadDpadRight };
        for (ImGuiKey k : keys) ImGui::SetKeyOwner(k, itemId);
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow) || ImGui::IsKeyPressed(ImGuiKey_GamepadDpadLeft))   *v -= step;
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow) || ImGui::IsKeyPressed(ImGuiKey_GamepadDpadRight)) *v += step;
    }
    if (resetValue != FLT_MAX && hovered && (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)))
        *v = resetValue;
    *v = ImClamp(*v, ImMin(vMin, vMax), ImMax(vMin, vMax));
    const bool changed = (*v != old);
    if (changed) ImFormatString(buf, IM_ARRAYSIZE(buf), format, *v);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    const float cy = p.y + H * 0.5f, th = M::sliderTrack * dpi * 0.5f;
    const float t = (vMax != vMin) ? ImClamp((*v - vMin) / (vMax - vMin), 0.0f, 1.0f) : 0.0f;
    const float kx = x0 + t * (x1 - x0);
    dl->AddRectFilled(ImVec2(p.x, cy - th), ImVec2(p.x + trackW, cy + th), detail::U32(col.bgFrameActive), th);
    dl->AddRectFilled(ImVec2(p.x, cy - th), ImVec2(kx, cy + th), detail::U32(col.accent), th);
    const ImVec4 ring = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
    dl->AddCircleFilled(ImVec2(kx, cy), knobR + 2 * dpi, detail::U32(ring));
    dl->AddCircleFilled(ImVec2(kx, cy), knobR, detail::U32((hovered || active) ? col.accentHover : col.accent));

    ImGui::SameLine(0, 0);
    const ImVec2 vp = ImGui::GetCursorScreenPos();
    ImGui::Dummy(ImVec2(fullW - trackW, H));
    PushTextStyle(TextStyle_Value);
    const ImVec2 ts2 = ImGui::CalcTextSize(buf);
    detail::DrawText(ImVec2(vp.x + (fullW - trackW) - ts2.x, cy - ImGui::GetFontSize() * 0.5f), col.textSecondary, buf);
    PopTextStyle();
    return changed;
}

inline bool SliderInt(const char* id, int* v, int vMin, int vMax, const char* format = "%d", int resetValue = INT_MIN) {
    float f = (float)*v;
    char fmt[32];
    // Reutiliza SliderFloat mostrando el valor entero: "%d" pasa a "%.0f".
    const char* src = format; char* dst = fmt;
    while (*src && dst < fmt + sizeof(fmt) - 4) {
        if (src[0] == '%' && src[1] == 'd') { *dst++ = '%'; *dst++ = '.'; *dst++ = '0'; *dst++ = 'f'; src += 2; }
        else *dst++ = *src++;
    }
    *dst = 0;
    const float step = (vMax - vMin) >= 100 ? (float)(vMax - vMin) / 100.0f : 1.0f;
    SliderFloat(id, &f, (float)vMin, (float)vMax, fmt, resetValue == INT_MIN ? FLT_MAX : (float)resetValue, step);
    const int nv = (int)(f + (f >= 0 ? 0.5f : -0.5f));
    const bool changed = (nv != *v);
    *v = nv;
    return changed;
}

// ---------------------------------------------------------------------------
// Desplegable (docs/componentes/Combo.md). Para 5 o más opciones; con 2 a 4 usa Segmented().
// ---------------------------------------------------------------------------
inline bool Combo(const char* id, int* v, const char* const items[], int count) {
    const Palette& col = Col();
    const float dpi = Dpi();
    const float H = M::controlH * dpi;
    bool changed = false;
    const float pad = M::space1 * dpi;

    // Caja cerrada sin botón de flecha: el chevron se dibuja después, en text-secondary.
    ImDrawList* frameDl = ImGui::GetWindowDrawList();
    const ImVec2 fp = ImGui::GetCursorScreenPos();
    const float fw = ImGui::CalcItemWidth();
    const float chevron = ImGui::GetFontSize() * 0.35f;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(M::space3 * dpi, ImMax(0.0f, (H - ImGui::GetFontSize()) * 0.5f)));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(pad, pad));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, M::radiusLg * dpi);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, ImMin(count, 8) * H + 2 * pad));   // hasta 8 opciones sin scroll
    const char* preview = (*v >= 0 && *v < count) ? items[*v] : "";
    const bool open = ImGui::BeginCombo(id, preview, ImGuiComboFlags_NoArrowButton);
    ImGui::PopStyleVar(3);

    const ImVec2 cc(fp.x + fw - M::space3 * dpi - chevron, fp.y + H * 0.5f);
    const ImVec2 chev[3] = { ImVec2(cc.x - chevron, cc.y - chevron * 0.5f), ImVec2(cc.x, cc.y + chevron * 0.5f), ImVec2(cc.x + chevron, cc.y - chevron * 0.5f) };
    frameDl->AddPolyline(chev, 3, detail::U32(col.textSecondary), ImDrawFlags_None, ImMax(1.0f, 1.5f * dpi));

    if (open) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImDrawList* dl = ImGui::GetWindowDrawList();
        for (int i = 0; i < count; i++) {
            const bool sel = (i == *v);
            ImGui::PushID(i);
            if (ImGui::Selectable("##opcion", sel, ImGuiSelectableFlags_None, ImVec2(0, H)) && !sel) { *v = i; changed = true; }
            const ImVec2 mn = ImGui::GetItemRectMin(), mx = ImGui::GetItemRectMax();
            detail::DrawText(ImVec2(mn.x + M::space3 * dpi, mn.y + (H - ImGui::GetFontSize()) * 0.5f), col.textPrimary, items[i]);
            if (sel) {
                ImGui::SetItemDefaultFocus();
                const float s = ImGui::GetFontSize() * 0.75f;
                ImGui::RenderCheckMark(dl, ImVec2(mx.x - M::space3 * dpi - s, mn.y + (H - s) * 0.5f), detail::U32(col.accent), s);
            }
            ImGui::PopID();
        }
        ImGui::PopStyleVar();
        ImGui::EndCombo();
    }
    return changed;
}

// ---------------------------------------------------------------------------
// Aviso (docs/componentes/Notice.md). Título = qué pasó; cuerpo = por qué y qué hacer; código solo en errores.
// ---------------------------------------------------------------------------
inline void Notice(Tone tone, const char* title, const char* body = nullptr, const char* code = nullptr) {
    const Palette& col = Col();
    const float dpi = Dpi();
    ImVec4 bg, fg; ToneColors(tone, &bg, &fg);
    if (tone == Tone_Neutral) fg = col.textSecondary;

    const ImVec2 p = ImGui::GetCursorScreenPos();
    const float w = ImGui::GetContentRegionAvail().x;
    const float padX = M::space4 * dpi, padY = M::space3 * dpi, icon = M::iconSize * dpi, gap = M::space1 * dpi;
    const float textX = p.x + padX + icon + M::space3 * dpi;
    const float textW = ImMax(1.0f, p.x + w - padX - textX);

    PushTextStyle(TextStyle_Label);  const float titleH = ImGui::CalcTextSize(title, nullptr, false, textW).y; const float lineH = ImGui::GetFontSize(); PopTextStyle();
    float bodyH = 0, codeH = 0;
    if (body) { PushTextStyle(TextStyle_Body);  bodyH = ImGui::CalcTextSize(body, nullptr, false, textW).y; PopTextStyle(); }
    if (code) { PushTextStyle(TextStyle_Value); codeH = ImGui::CalcTextSize(code, nullptr, false, textW).y; PopTextStyle(); }
    const float h = 2 * padY + titleH + (body ? gap + bodyH : 0) + (code ? gap + codeH : 0);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), detail::U32(bg), M::radiusLg * dpi);

    // Ícono: círculo del color de estado con ✓ (correcto), ! (error/advertencia) o i (neutro).
    const ImVec2 c(p.x + padX + icon * 0.5f, p.y + padY + lineH * 0.5f);
    const float r = icon * 0.5f;
    dl->AddCircleFilled(c, r, detail::U32(fg));
    if (tone == Tone_Success) {
        const float s = icon * 0.55f;
        ImGui::RenderCheckMark(dl, ImVec2(c.x - s * 0.5f, c.y - s * 0.5f), detail::U32(bg), s);
    } else {
        const float lw = ImMax(1.5f, 1.75f * dpi);
        const bool info = (tone == Tone_Neutral);
        const float y0 = info ? c.y - r * 0.05f : c.y - r * 0.5f, y1 = info ? c.y + r * 0.5f : c.y + r * 0.1f;
        dl->AddLine(ImVec2(c.x, y0), ImVec2(c.x, y1), detail::U32(bg), lw);
        dl->AddCircleFilled(ImVec2(c.x, info ? c.y - r * 0.42f : c.y + r * 0.45f), lw * 0.6f, detail::U32(bg));
    }

    float y = p.y + padY;
    PushTextStyle(TextStyle_Label); detail::DrawText(ImVec2(textX, y), col.textPrimary, title, nullptr, textW); PopTextStyle();
    y += titleH;
    if (body) { y += gap; PushTextStyle(TextStyle_Body);  detail::DrawText(ImVec2(textX, y), col.textPrimary, body, nullptr, textW); PopTextStyle(); y += bodyH; }
    if (code) { y += gap; PushTextStyle(TextStyle_Value); detail::DrawText(ImVec2(textX, y), col.textSecondary, code, nullptr, textW); PopTextStyle(); }
    ImGui::Dummy(ImVec2(w, h));
}

// ---------------------------------------------------------------------------
// Modal de confirmación (docs/componentes/ConfirmModal.md).
// Ábrelo con ImGui::OpenPopup(id) y llama a ConfirmModal() cada frame.
// Devuelve ConfirmResult_Confirm, ConfirmResult_Cancel o ConfirmResult_None. Esc y B del mando cancelan.
// Sin 'cancelLabel', el botón dice «Cancelar» en el idioma activo.
// ---------------------------------------------------------------------------
enum ConfirmResult { ConfirmResult_None, ConfirmResult_Confirm, ConfirmResult_Cancel };

inline ConfirmResult ConfirmModal(const char* id, const char* title, const char* body, const char* confirmLabel,
                                  bool destructive = true, const char* cancelLabel = nullptr) {
    if (!cancelLabel) cancelLabel = Tr("Cancelar", "Cancel", "Cancelar");
    const Palette& col = Col();
    const float dpi = Dpi();
    const ImGuiViewport* vp = ImGui::GetMainViewport();
    const float width = ImMin(M::modalW * dpi, vp->WorkSize.x - 2 * M::space6 * dpi);
    ImGui::SetNextWindowPos(vp->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(width, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(M::space5 * dpi, M::space5 * dpi));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, M::radiusXl * dpi);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, col.bgPopup);
    const bool open = ImGui::BeginPopupModal(id, nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);
    if (!open) return ConfirmResult_None;

    ConfirmResult r = ConfirmResult_None;
    const float spacing = ImGui::GetStyle().ItemSpacing.y;
    PushTextStyle(TextStyle_PageTitle);
    ImGui::TextWrapped("%s", title);
    PopTextStyle();
    if (body) {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - spacing + M::space2 * dpi);
        ImGui::PushStyleColor(ImGuiCol_Text, col.textSecondary);
        ImGui::TextWrapped("%s", body);
        ImGui::PopStyleColor();
    }
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - spacing + M::space5 * dpi);

    const float gap = M::space2 * dpi;
    const float total = ButtonWidth(cancelLabel) + gap + ButtonWidth(confirmLabel);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImMax(0.0f, ImGui::GetContentRegionAvail().x - total));
    if (GhostButton(cancelLabel)) r = ConfirmResult_Cancel;
    ImGui::SetItemDefaultFocus();   // el foco inicial va en Cancelar
    ImGui::SameLine(0, gap);
    if (ButtonEx(confirmLabel, destructive ? ButtonKind_Danger : ButtonKind_Primary)) r = ConfirmResult_Confirm;

    if (!ImGui::IsWindowAppearing() && (ImGui::IsKeyPressed(ImGuiKey_Escape, false) || ImGui::IsKeyPressed(ImGuiKey_GamepadFaceRight, false)))
        r = ConfirmResult_Cancel;
    if (r != ConfirmResult_None) ImGui::CloseCurrentPopup();
    ImGui::EndPopup();
    return r;
}

// ---------------------------------------------------------------------------
// Ventana de ajustes (docs/componentes/SettingsWindow.md) y barra lateral (docs/componentes/SidebarNav.md).
//
//   if (BeginSettingsWindow("##ajustes", &open)) {
//       BeginSidebar("Ajustes");
//       if (SidebarItem("Video", cat == 0)) cat = 0;
//       ...
//       EndSidebar();
//       if (BeginSettingsContent("Video")) { ...filas... }
//       EndSettingsContent();
//       BeginSettingsFooter();
//       GhostButton("Ayuda");
//       SameLineRight(ButtonWidth("Aplicar")); PrimaryButton("Aplicar");
//       EndSettingsFooter();
//   }
//   EndSettingsWindow();
// ---------------------------------------------------------------------------
inline bool BeginSettingsWindow(const char* name, bool* open = nullptr) {
    const float dpi = Dpi();
    const ImGuiViewport* vp = ImGui::GetMainViewport();
    const ImVec2 size(ImMin(M::windowW * dpi, vp->WorkSize.x - 2 * M::space6 * dpi),
                      ImMin(M::windowH * dpi, vp->WorkSize.y - 2 * M::space6 * dpi));
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::SetNextWindowPos(vp->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    const bool visible = ImGui::Begin(name, open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings);
    ImGui::PopStyleVar();
    return visible;
}
namespace detail { inline bool& InSidebarGroup() { static bool b = false; return b; } }
inline void EndSettingsWindow() {
    if (detail::InSidebarGroup()) { ImGui::EndGroup(); detail::InSidebarGroup() = false; }
    ImGui::End();
}

inline void BeginSidebar(const char* title) {
    const float dpi = Dpi();
    const ImVec2 wp = ImGui::GetWindowPos();
    ImGui::GetWindowDrawList()->AddRectFilled(wp, ImVec2(wp.x + M::sidebarW * dpi, wp.y + ImGui::GetWindowHeight()),
        detail::U32(Col().bgSidebar), ImGui::GetStyle().WindowRounding, ImDrawFlags_RoundCornersLeft);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(M::space2 * dpi, M::space4 * dpi));
    ImGui::BeginChild("##sidebar", ImVec2(M::sidebarW * dpi, 0), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar);
    ImGui::PopStyleVar();

    // Fondo del ítem seleccionado: se desliza hasta la posición que SidebarItem() guardó el frame anterior.
    ImGuiStorage* st = ImGui::GetStateStorage();
    const float target = st->GetFloat(detail::WindowKey("moderno.nav.target"), -1.0f);
    if (target >= 0.0f) {
        const ImGuiID keyY = detail::WindowKey("moderno.nav.y");
        const float prev = st->GetFloat(keyY, -1.0f);
        const float y = prev < 0.0f ? target : detail::Approach(prev, target, 0.25f);
        st->SetFloat(keyY, y);
        const ImVec2 cp = ImGui::GetCursorScreenPos(), win = ImGui::GetWindowPos();
        const float w = ImGui::GetContentRegionAvail().x;
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cp.x, win.y + y), ImVec2(cp.x + w, win.y + y + M::navItemH * dpi),
            detail::U32(Col().accentSoft), M::radiusLg * dpi);
    }
    st->SetFloat(detail::WindowKey("moderno.nav.target"), -1.0f);   // si ningún ítem queda seleccionado, no se dibuja

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(M::space2 * dpi, M::navItemGap * dpi));
    if (title) {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + M::space3 * dpi);
        PushTextStyle(TextStyle_WindowTitle);
        ImGui::TextUnformatted(title);
        PopTextStyle();
        ImGui::Dummy(ImVec2(0, M::space4 * dpi - 2 * M::navItemGap * dpi));
    }
}

namespace detail {
    inline bool SidebarItemImpl(const char* label, bool selected, const char* glyph, Icon icon) {
        const Palette& col = Col();
        const float dpi = Dpi();
        const ImVec2 p = ImGui::GetCursorScreenPos();
        const ImVec2 size(ImGui::GetContentRegionAvail().x, M::navItemH * dpi);
        const bool pressed = ImGui::InvisibleButton(label, size);
        const bool hovered = ImGui::IsItemHovered();
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImGuiStorage* st = ImGui::GetStateStorage();
        if (selected) {
            st->SetFloat(WindowKey("moderno.nav.target"), p.y - ImGui::GetWindowPos().y);
            if (st->GetFloat(WindowKey("moderno.nav.y"), -1.0f) < 0.0f)   // primer frame: aún no hay fondo animado
                dl->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), U32(col.accentSoft), M::radiusLg * dpi);
        } else if (hovered) {
            dl->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), U32(col.bgFrameHover), M::radiusLg * dpi);
        }
        const ImVec4& text = (selected || hovered) ? col.textPrimary : col.textSecondary;
        const ImVec4& iconCol = selected ? col.accent : text;
        float x = p.x + M::space3 * dpi;
        PushTextStyle(TextStyle_Label);
        const float ty = p.y + (size.y - ImGui::GetFontSize()) * 0.5f;
        if (icon != Icon_None) {
            const float isz = M::iconSize * dpi;
            DrawIcon(dl, icon, ImVec2(x, p.y + (size.y - isz) * 0.5f), isz, U32(iconCol));
            x += isz + M::space2 * dpi;
        } else if (glyph) {
            DrawText(ImVec2(x, ty), iconCol, glyph);
            x += M::iconSize * dpi + M::space2 * dpi;
        }
        DrawText(ImVec2(x, ty), text, label, ImGui::FindRenderedTextEnd(label));
        PopTextStyle();
        return pressed;
    }
}

// Ítem de navegación de 36 px con un ícono del sistema (Icon_Monitor, Icon_Palette…).
inline bool SidebarItem(const char* label, bool selected, Icon icon) { return detail::SidebarItemImpl(label, selected, nullptr, icon); }
// Variante con un glifo de una fuente de íconos ya fusionada (opcional).
inline bool SidebarItem(const char* label, bool selected, const char* glyph = nullptr) { return detail::SidebarItemImpl(label, selected, glyph, Icon_None); }

inline void SidebarSeparator() {
    const float dpi = Dpi();
    const float gap = M::navItemGap * dpi;
    ImGui::Dummy(ImVec2(0, M::space2 * dpi - 2 * gap));
    const ImVec2 p = ImGui::GetCursorScreenPos();
    const float w = ImGui::GetContentRegionAvail().x;
    ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + M::space3 * dpi, p.y), ImVec2(p.x + w - M::space3 * dpi, p.y), detail::U32(Col().separator), 1.0f);
    ImGui::Dummy(ImVec2(w, 1));
    ImGui::Dummy(ImVec2(0, M::space2 * dpi - 2 * gap));
}

inline void EndSidebar() {
    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::SameLine(0, 0);
    ImGui::BeginGroup();   // contenido y barra de acciones a la derecha de la barra lateral; lo cierra EndSettingsWindow()
    detail::InSidebarGroup() = true;
}

// Área de contenido con scroll y relleno de 24 px. Llamar siempre a EndSettingsContent().
inline bool BeginSettingsContent(const char* pageTitle, bool hasFooter = true) {
    const float dpi = Dpi();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(M::space5 * dpi, M::space5 * dpi));
    // Reserva la barra de acciones más el ItemSpacing que ImGui añade tras el child; si no, la barra se sale por abajo.
    const float footer = hasFooter ? M::footerH * dpi + ImGui::GetStyle().ItemSpacing.y : 0.0f;
    const bool visible = ImGui::BeginChild("##contenido", ImVec2(0, -footer), ImGuiChildFlags_AlwaysUseWindowPadding);
    ImGui::PopStyleVar();
    if (visible && pageTitle) {
        PushTextStyle(TextStyle_PageTitle);
        ImGui::TextUnformatted(pageTitle);
        PopTextStyle();
        ImGui::Dummy(ImVec2(0, ImMax(0.0f, M::space4 * dpi - 2 * ImGui::GetStyle().ItemSpacing.y)));   // page-title-gap
    }
    return visible;
}
inline void EndSettingsContent() { ImGui::EndChild(); }

// Barra de acciones de 56 px: ayuda a la izquierda; Restablecer y Aplicar a la derecha (usa SameLineRight).
inline void BeginSettingsFooter() {
    const float dpi = Dpi();
    const ImVec2 p = ImGui::GetCursorScreenPos();
    const float w = ImGui::GetContentRegionAvail().x;
    ImGui::GetWindowDrawList()->AddLine(p, ImVec2(p.x + w, p.y), detail::U32(Col().separator), 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(M::space5 * dpi, (M::footerH - M::controlH) * 0.5f * dpi));
    ImGui::BeginChild("##acciones", ImVec2(0, M::footerH * dpi), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PopStyleVar();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(M::space2 * dpi, 0));
}
inline void EndSettingsFooter() {
    ImGui::PopStyleVar();
    ImGui::EndChild();
}

// Demo completa: ventana de ajustes de un emulador con todos los componentes (imgui_moderno_demo.cpp).
void ShowDemoWindow(bool* open = nullptr);

} // namespace ImGuiModerno
