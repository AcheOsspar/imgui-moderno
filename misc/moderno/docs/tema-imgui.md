# Tema ImGui

Cómo se aplican los tokens de este sistema a Dear ImGui (1.92 o posterior). Todo está implementado en `ImGuiModerno::ApplyTheme(dark, dpi)` de `imgui_moderno.h`; este documento explica qué hace para que puedas revisarlo o portarlo.

Llama a `ApplyTheme(dark, dpi)` una vez al iniciar y cada vez que cambie el modo o la escala, fuera de cualquier `Push`/`Pop` de estilo. Reemplaza a `ImGui::StyleColorsDark()` y a `style.ScaleAllSizes()`.

## Paleta

```cpp
struct Palette {
    ImVec4 bgWindow, bgSidebar, bgPopup, bgFrame, bgFrameHover, bgFrameActive, bgOverlay;
    ImVec4 textPrimary, textSecondary, textMuted, textDisabled;
    ImVec4 accent, accentHover, accentSoft, onAccent;
    ImVec4 borderSubtle, separator, knob, knobOff, scrollbarGrab;
    ImVec4 success, successSoft, warning, warningSoft, danger, dangerSoft;
};
```

Los valores de `Dark()` y `Light()` son los de `tokens.json` → `color.tokens`. La paleta activa se lee con `ImGuiModerno::Col()`; los widgets propios la usan para todo lo que dibujan.

## Qué hace `ApplyTheme`

1. Copia la paleta del modo en `Col()` y guarda `Dpi()` e `IsDark()`.
2. Reinicia el estilo (`style = ImGuiStyle()`) para que llamarla varias veces no acumule escalas.
3. Aplica los tokens de `ImGuiStyle`:

| Campo | Valor | Token / motivo |
|---|---|---|
| `WindowPadding` | 0 × 0 | La ventana no tiene relleno; lo ponen la barra lateral y el contenido |
| `FramePadding` | 12 × 6 | `combo-padding`; los widgets fijan su alto a 32 px |
| `ItemSpacing` | 8 × 8 | `space-2` |
| `ItemInnerSpacing` | 8 × 4 | |
| `CellPadding` | 0 × 12 | `row-padding` |
| `IndentSpacing` | 24 | `row-indent` |
| `ScrollbarSize` | 10 | |
| `GrabMinSize` | 14 | `slider-knob` |
| `WindowBorderSize`, `ChildBorderSize`, `FrameBorderSize`, `TabBorderSize` | 0 | Sin bordes: separa el fondo |
| `PopupBorderSize`, `SeparatorTextBorderSize` | 1 | Popups con `border-subtle` |
| `WindowRounding` | 12 | `radius-xl` |
| `ChildRounding`, `PopupRounding` | 8 | `radius-lg` |
| `FrameRounding`, `TabRounding` | 6 | `radius-md` |
| `GrabRounding`, `ScrollbarRounding` | 999 | `radius-full` |
| `WindowTitleAlign`, `SelectableTextAlign` | 0 × 0.5 | Texto a la izquierda, centrado en vertical |
| `HoverDelayShort` | 0.40 s | Retraso del tooltip |
| `FontSizeBase` | 15 | Estilo `body` |
| `FontScaleDpi` | `dpi` | Escala de las fuentes |

4. Escala todas las medidas con `style.ScaleAllSizes(dpi)`.
5. Asigna los colores:

| `ImGuiCol_` | Token |
|---|---|
| `WindowBg` | `bg-window` |
| `ChildBg`, `BorderShadow`, `ScrollbarBg`, `ResizeGrip`, `TableRowBg`, `TableRowBgAlt` | transparente |
| `PopupBg` | `bg-popup` |
| `Border` | `border-subtle` |
| `Text` / `TextDisabled` | `text-primary` / `text-disabled` |
| `TextSelectedBg`, `Header`, `HeaderActive` | `accent-soft` |
| `FrameBg` / `FrameBgHovered` / `FrameBgActive` | `bg-frame` / `bg-frame-hover` / `bg-frame-active` |
| `Button` / `ButtonHovered` / `ButtonActive` | `bg-frame` / `bg-frame-hover` / `bg-frame-active` |
| `HeaderHovered`, `TabHovered` | `bg-frame-hover` |
| `TitleBg`, `TitleBgActive`, `TitleBgCollapsed` | `bg-window` |
| `MenuBarBg`, `Tab`, `TableHeaderBg` | `bg-sidebar` |
| `TabSelected` | `bg-frame` |
| `ScrollbarGrab` / `Hovered` / `Active` | `scrollbar-grab` / `bg-frame-active` / `text-muted` |
| `CheckMark`, `SliderGrab`, `SeparatorHovered`, `SeparatorActive`, `ResizeGripActive`, `TabSelectedOverline`, `NavCursor`, `DragDropTarget` | `accent` |
| `SliderGrabActive` | `accent-hover` |
| `Separator`, `TableBorderStrong`, `TableBorderLight` | `separator` |
| `ResizeGripHovered` | `bg-frame-active` |
| `ModalWindowDimBg`, `NavWindowingDimBg` | `bg-overlay` |

Si cambias cualquiera de estos valores, cámbialo también en `tokens.json` (`imgui.tokens` o `color.tokens`).

## Fuentes

Desde Dear ImGui 1.92 las fuentes son dinámicas: cada archivo se carga una vez y el tamaño se elige al dibujar. El sistema usa cuatro archivos:

```cpp
ImGuiModerno::LoadFonts(
    "fonts/Inter-Regular.ttf",            // body, caption (y fuente por defecto)
    "fonts/Inter-Medium.ttf",             // label, badge
    "fonts/Inter-SemiBold.ttf",           // section, page-title, window-title
    "fonts/JetBrainsMono-Regular.ttf");   // value
```

- Las rutas que no existan se ignoran: si falta Inter, se usa la fuente por defecto de ImGui; si falta Medium, se usa Regular; si falta SemiBold, Medium.
- El tamaño y la fuente de cada texto se eligen con `PushTextStyle(TextStyle_…)` / `PopTextStyle()`:

| `TextStyle_` | Fuente | Tamaño |
|---|---|---|
| `Body` | Regular | 15 |
| `Label` | Medium | 15 |
| `Caption` | Regular | 13 |
| `Section` | SemiBold | 12 |
| `PageTitle` | SemiBold | 17 |
| `WindowTitle` | SemiBold | 20 |
| `Value` | Mono | 13 |
| `Badge` | Medium | 12 |

- No multipliques los tamaños por el DPI: lo hace `style.FontScaleDpi`, que fija `ApplyTheme`.

### Íconos (opcional)

Para usar Lucide, fusiona su fuente con Inter Regular y Medium justo después de cargar cada una. Con fuentes dinámicas no hace falta indicar tamaños:

```cpp
ImGuiIO& io = ImGui::GetIO();
ImFontConfig merge; merge.MergeMode = true; merge.GlyphMinAdvanceX = 16.0f;
ImGuiModerno::LoadFonts("fonts/Inter-Regular.ttf", nullptr, nullptr, nullptr);
io.Fonts->AddFontFromFileTTF("fonts/lucide.ttf", 0.0f, &merge);          // fusionada en Regular
ImGuiModerno::Fonts().medium = io.Fonts->AddFontFromFileTTF("fonts/Inter-Medium.ttf");
io.Fonts->AddFontFromFileTTF("fonts/lucide.ttf", 0.0f, &merge);          // fusionada en Medium
ImGuiModerno::Fonts().semibold = io.Fonts->AddFontFromFileTTF("fonts/Inter-SemiBold.ttf");
ImGuiModerno::Fonts().mono = io.Fonts->AddFontFromFileTTF("fonts/JetBrainsMono-Regular.ttf");

ImGuiModerno::SidebarItem("Video", cat == 0, ICON_LC_MONITOR);
```

Todas las fuentes son sans-serif o monoespaciadas; el sistema no usa fuentes serif.

## Checklist de implementación

- [ ] `ApplyTheme` se llama al iniciar y al cambiar modo o escala; no queda ningún `StyleColors*()` ni `ScaleAllSizes()` aparte.
- [ ] Las fuentes se cargan con `LoadFonts` y se eligen con `PushTextStyle`.
- [ ] La ventana de ajustes usa `BeginSettingsWindow` (sin barra de título; dibuja su propio encabezado en la barra lateral).
- [ ] Cada ajuste usa `SettingRow` dentro de `BeginSettingRows`.
- [ ] Los booleanos usan `ToggleSwitch`, no `Checkbox`.
- [ ] Cada opción deshabilitada explica por qué.
- [ ] La navegación con mando funciona en toda la ventana (`ImGuiConfigFlags_NavEnableGamepad`).
