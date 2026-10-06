# Referencia de la API (`imgui_moderno.h`)

Todo vive en el namespace `ImGuiModerno`. Header-only; requiere Dear ImGui **1.92 o posterior** (usa `PushFont(font, size)`). Ningún widget recibe medidas: todas salen de los tokens (`M::*`) multiplicados por `Dpi()`.

## 1. Arranque

| Función | Qué hace |
|---|---|
| `void LoadFonts(const char* regular, const char* medium, const char* semibold, const char* mono)` | Carga Inter Regular/Medium/SemiBold y JetBrains Mono. Llamar una vez antes del primer frame. Las rutas inexistentes (o `nullptr`) se ignoran y esos estilos usan la fuente por defecto. La primera fuente cargada pasa a ser la predeterminada. |
| `void ApplyTheme(bool dark, float dpi)` | Reinicia `ImGuiStyle`, aplica todos los tokens, los colores del modo y escala por `dpi` (`ScaleAllSizes` + `FontScaleDpi`). Reemplaza a `ImGui::StyleColorsDark()`. Llamar al iniciar y cada vez que cambie el modo o la escala, **fuera de cualquier `Push`/`Pop` de estilo**. |

```cpp
ImGui::CreateContext();
ImGuiModerno::LoadFonts("fonts/Inter-Regular.ttf", "fonts/Inter-Medium.ttf",
                        "fonts/Inter-SemiBold.ttf", "fonts/JetBrainsMono-Regular.ttf");
ImGuiModerno::ApplyTheme(/*dark=*/true, /*dpi=*/main_scale);
```

## 2. Estado y color

| Símbolo | Uso |
|---|---|
| `Palette& Col()` | Paleta activa (`bgWindow`, `textSecondary`, `accent`, `danger`…). **Todo color sale de aquí**; nunca escribas colores sueltos. |
| `float& Dpi()` | Escala activa. Multiplica por ella cualquier medida propia. |
| `bool& IsDark()` | Modo activo. |
| `const Palette& Dark()`, `Light()` | Paletas de cada modo (las que copia `ApplyTheme`). |
| `ImVec4 Hex(uint32_t rgb, float a = 1)` | Convierte `0xRRGGBB` a `ImVec4`. Solo para definir paletas. |
| `enum Tone { Tone_Neutral, Tone_Success, Tone_Warning, Tone_Danger }` | Tono de insignias y avisos. |
| `void ToneColors(Tone, ImVec4* bg, ImVec4* fg)` | Fondo suave y color de texto de un tono. |
| `namespace M` | Tokens numéricos a escala 1: `space1`…`space6` (4–32), `controlH` 32, `segH` 28, `badgeH` 20, `navItemH` 36, `rowMinH` 44, `footerH` 56, `sidebarW` 200, `controlW` 240, `windowW` 880, `windowH` 600, `modalW` 400, `tooltipMaxW` 280, `radiusSm/Md/Lg/Xl/Full`, tamaños de fuente `fontLabel` 15, `fontCaption` 13… |

## 2b. Idiomas

Español, inglés y portugués (Brasil). Los textos propios del sistema (por ahora «Cancelar» de `ConfirmModal`) siguen el idioma activo; tus textos se traducen con `Tr()`.

| Símbolo | Uso |
|---|---|
| `enum Language { Language_Spanish, Language_English, Language_Portuguese }` | Idiomas disponibles. |
| `void SetLanguage(Language)` / `Language& CurrentLanguage()` | Cambia o lee el idioma activo. Se puede cambiar en cualquier momento, incluso a mitad de frame. |
| `const char* Tr(const char* es, const char* en, const char* pt)` | Devuelve el texto del idioma activo. |
| `const char* LanguageName(Language)` | «Español», «English», «Português»: cada nombre en su idioma, para el selector. |

```cpp
ImGuiModerno::SettingRow(ImGuiModerno::Tr("Campo de visión", "Field of view", "Campo de visão"),
                         ImGuiModerno::Tr("Amplía lo que ves a los lados.", "Shows more to the sides.", "Amplia o que você vê dos lados."));

// Selector de idioma (3 opciones → Segmented)
const char* const idiomas[] = { ImGuiModerno::LanguageName(ImGuiModerno::Language_Spanish),
                                ImGuiModerno::LanguageName(ImGuiModerno::Language_English),
                                ImGuiModerno::LanguageName(ImGuiModerno::Language_Portuguese) };
if (ImGuiModerno::Segmented("##idioma", &cfg.idioma, idiomas, 3))
    ImGuiModerno::SetLanguage((ImGuiModerno::Language)cfg.idioma);
```

**IDs estables.** En ImGui la etiqueta también es el ID del widget. Si una etiqueta traducida identifica algo con estado, fija el ID para que no cambie con el idioma:

- `CollapsibleSection(Tr("Avanzado###avanzado", "Advanced###avanzado", "Avançado###avanzado"))`: todo lo que va tras `###` es el ID y no se muestra.
- En bucles (`SidebarItem`), envuelve cada ítem en `ImGui::PushID(i)` / `ImGui::PopID()`.
- Los controles de las filas ya usan IDs ocultos (`"##fov"`), que no se traducen.

Si tu proyecto ya usa su propio sistema de traducción (gettext, archivos JSON…), no necesitas `Tr()`: pasa a los widgets el texto ya traducido. Llama igualmente a `SetLanguage()` para que los textos del sistema coincidan.

## 3. Tipografía

| Símbolo | Uso |
|---|---|
| `enum TextStyle` | `TextStyle_Body` 15, `TextStyle_Label` 15 medium, `TextStyle_Caption` 13, `TextStyle_Section` 12 semibold, `TextStyle_PageTitle` 17 semibold, `TextStyle_WindowTitle` 20 semibold, `TextStyle_Value` 13 mono, `TextStyle_Badge` 12 medium. |
| `void PushTextStyle(TextStyle)` / `void PopTextStyle()` | Cambia fuente y tamaño a la vez. Funciona aunque no se hayan cargado fuentes (solo cambia el tamaño). |
| `FontSet& Fonts()` | Las fuentes cargadas (`regular`, `medium`, `semibold`, `mono`); pueden ser `nullptr`. |

## 4. Filas de ajuste

```cpp
if (ImGuiModerno::BeginSettingRows("##estilo")) {
    ImGuiModerno::SettingRow("Contornos", "Bordes oscuros en personajes y objetos.");
    ImGuiModerno::ToggleSwitch("##contornos", &cfg.outlines);
    ImGuiModerno::EndSettingRows();
}
```

| Función | Qué hace |
|---|---|
| `bool BeginSettingRows(const char* id)` | Abre la tabla de 2 columnas (texto elástico + control de 240 px) con línea entre filas. Si devuelve `false`, no llames a `EndSettingRows()`. |
| `void SettingRow(const char* label, const char* desc = nullptr, const RowOptions& opt = RowOptions())` | Dibuja etiqueta (`label`) y descripción (`caption`, `textSecondary`), centra el bloque con el control y deja el cursor en la columna derecha con `SetNextItemWidth(-FLT_MIN)`. **Debe ir seguida de exactamente un control.** |
| `void EndSettingRows()` | Cierra la tabla. |
| `struct RowOptions` | `badge` (texto), `badgeTone` (por defecto `Tone_Warning`), `helpTitle` / `helpBody` (ícono «?» con tooltip), `dependent` (sangría de 24 px para opciones hijas). |

Opción deshabilitada: envuelve fila **y** control en `ImGui::BeginDisabled(cond)` / `ImGui::EndDisabled()` y explica el motivo en `desc`.

```cpp
ImGuiModerno::RowOptions dep; dep.dependent = true;
ImGui::BeginDisabled(!cfg.interpolation);
ImGuiModerno::SettingRow("Cuadros intermedios",
    cfg.interpolation ? "Más cuadros, más fluidez y más consumo." : "Activa Interpolación visual para elegir cuántos.", dep);
ImGuiModerno::Segmented("##frames", &cfg.frames, frames, 2);
ImGui::EndDisabled();
```

**Regla de alineación.** Dentro de la columna de control (es decir, con `SetNextItemWidth` activo), `ToggleSwitch`, `Badge`, `Value` y los botones se pegan al borde derecho y se centran en 32 px; `Segmented`, `Combo` y `SliderFloat/Int` ocupan todo el ancho. Fuera de una fila se comportan como widgets normales de ImGui.

## 5. Controles

| Función | Devuelve | Uso |
|---|---|---|
| `bool ToggleSwitch(const char* id, bool* v)` | `true` al cambiar | Sí/no. Nunca `ImGui::Checkbox` (salvo listas de selección múltiple). |
| `bool Segmented(const char* id, int* v, const char* const items[], int count)` | `true` al cambiar | 2 a 4 opciones cortas. `*v = -1` deja todo sin marcar (estado «Personalizado»). |
| `bool Combo(const char* id, int* v, const char* const items[], int count)` | `true` al cambiar | 5 o más opciones. Hasta 8 visibles sin scroll. |
| `bool SliderFloat(const char* id, float* v, float min, float max, const char* fmt = "%.2f", float reset = FLT_MAX, float step = 0)` | `true` al cambiar | Valores continuos. `fmt` incluye la unidad (`"%.0f %%"`, `"%.2fx"`). Con `reset`, clic derecho o doble clic restauran. Izquierda/derecha (teclado o cruceta) mueven `step` (por defecto 1 % del rango). |
| `bool SliderInt(const char* id, int* v, int min, int max, const char* fmt = "%d", int reset = INT_MIN)` | `true` al cambiar | Igual, con enteros. |
| `bool PrimaryButton(const char* label, Icon icon = Icon_None)` | `true` al pulsar | La acción principal. **Uno por vista.** |
| `bool SecondaryButton(const char* label, Icon icon = Icon_None)` | idem | Acciones normales («Elegir ROM»). |
| `bool GhostButton(const char* label, Icon icon = Icon_None)` | idem | Acciones de poco peso («Restablecer», «Cancelar», «Ayuda»). |
| `bool DangerButton(const char* label, Icon icon = Icon_None)` | idem | Destructivas. **Siempre** abren un `ConfirmModal`. |
| `bool ButtonEx(const char* label, ButtonKind kind, float width = 0, Icon icon = Icon_None)` | idem | Variante con ancho fijo. |
| `float ButtonWidth(const char* label, Icon icon = Icon_None)` | ancho | Para alinear grupos de botones a la derecha (pasa el mismo `icon` que al botón). |

El ícono opcional va a la izquierda del texto, a 8 px: `GhostButton("Ayuda", Icon_Help)`, `DangerButton("Borrar perfil", Icon_Trash)`.
| `void SameLineRight(float width)` | — | Siguiente ítem en la misma línea, pegado a la derecha dejando `width`. |

## 6. Información

| Función | Uso |
|---|---|
| `void Badge(const char* text, Tone tone = Tone_Neutral)` | Insignia de 20 px. Una por fila como máximo; en filas usa `RowOptions::badge`. |
| `void Badge(const char* text, const ImVec4& bg, const ImVec4& fg)` | Con colores propios de la paleta. |
| `void Value(const char* text)` | Lectura en mono 13 px, `textSecondary` («v2.4.1», «90 Hz»). |
| `void Notice(Tone tone, const char* title, const char* body = nullptr, const char* code = nullptr)` | Aviso a todo el ancho. `title` = qué pasó; `body` = por qué y qué hacer; `code` = solo en errores. |
| `void ItemTooltip(const char* title, const char* body)` | Tooltip (400 ms, máx. 280 px) para el último ítem. `title` puede ser `nullptr`. |
| `bool BeginItemTooltip()` / `void EndItemTooltip()` | Tooltip con contenido propio, ya con relleno y ancho del sistema. |
| `void HelpMarker(const char* title, const char* body)` | Ícono «?» enfocable con mando, con tooltip. En filas usa `RowOptions::helpTitle/helpBody`. |
| `void SectionHeader(const char* text, bool first = false)` | Título de sección; `first = true` en la primera de la página (sin hueco arriba). Debe ir seguido de filas. |
| `bool CollapsibleSection(const char* text, bool defaultOpen = false, bool first = false)` | Sección colapsable para «Avanzado». `if (CollapsibleSection("Avanzado") && BeginSettingRows("##av")) { …; EndSettingRows(); }` |

## 7. Modal de confirmación

```cpp
if (ImGuiModerno::DangerButton("Borrar perfil")) ImGui::OpenPopup("##borrar");
switch (ImGuiModerno::ConfirmModal("##borrar", "¿Borrar el perfil «Alto 90 Hz»?",
            "Se eliminarán sus ajustes. Tus partidas guardadas no se verán afectadas.", "Borrar perfil")) {
case ImGuiModerno::ConfirmResult_Confirm: DeleteProfile(); break;
default: break;
}
```

`ConfirmResult ConfirmModal(const char* id, const char* title, const char* body, const char* confirmLabel, bool destructive = true, const char* cancelLabel = nullptr)`. Sin `cancelLabel`, el botón dice «Cancelar» en el idioma activo. Llamar **cada frame** (aunque no esté abierto) en la misma ventana donde se hizo `OpenPopup(id)`. Esc y B del mando cancelan; el foco inicial va en Cancelar.

## 8. Ventana de ajustes completa

```cpp
if (ImGuiModerno::BeginSettingsWindow("Ajustes##app", &open)) {
    ImGuiModerno::BeginSidebar("Ajustes");
    for (int i = 0; i < count; i++) {
        if (i == aboutIndex) ImGuiModerno::SidebarSeparator();
        if (ImGuiModerno::SidebarItem(names[i], cat == i)) cat = i;
    }
    ImGuiModerno::EndSidebar();

    if (ImGuiModerno::BeginSettingsContent(names[cat])) {
        DrawPage(cat);                       // secciones + filas
    }
    ImGuiModerno::EndSettingsContent();      // siempre

    ImGuiModerno::BeginSettingsFooter();
    ImGuiModerno::GhostButton("Ayuda");
    const float gap = ImGuiModerno::M::space2 * ImGuiModerno::Dpi();
    ImGuiModerno::SameLineRight(ImGuiModerno::ButtonWidth("Restablecer") + (dirty ? gap + ImGuiModerno::ButtonWidth("Aplicar") : 0));
    if (ImGuiModerno::GhostButton("Restablecer")) Reset();
    if (dirty) { ImGui::SameLine(0, gap); if (ImGuiModerno::PrimaryButton("Aplicar")) Apply(); }
    ImGuiModerno::EndSettingsFooter();
}
ImGuiModerno::EndSettingsWindow();           // siempre, como ImGui::End()
```

| Función | Notas |
|---|---|
| `bool BeginSettingsWindow(const char* name, bool* open = nullptr)` | 880 × 600 escalado, centrada, sin barra de título, sin mover ni redimensionar. Se ajusta si la pantalla es más chica. |
| `void EndSettingsWindow()` | Siempre, aunque `Begin` devuelva `false`. |
| `void BeginSidebar(const char* title)` / `void EndSidebar()` | Barra lateral de 200 px con su título. |
| `bool SidebarItem(const char* label, bool selected, Icon icon)` | Ítem de 36 px con un ícono del sistema (sección 9). El fondo del seleccionado se desliza al cambiar de categoría. |
| `bool SidebarItem(const char* label, bool selected, const char* glyph = nullptr)` | Igual, con un glifo de una fuente de íconos fusionada, o sin ícono. |
| `void SidebarSeparator()` | Separador antes de «Acerca de». |
| `bool BeginSettingsContent(const char* pageTitle, bool hasFooter = true)` / `void EndSettingsContent()` | Zona con scroll y relleno de 24 px. Reserva la barra de acciones **más** el `ItemSpacing.y` que ImGui añade tras el child (si no, la barra queda recortada y pegada al borde). |
| `void BeginSettingsFooter()` / `void EndSettingsFooter()` | Barra de 56 px con línea superior; botones de 32 px centrados (12 px arriba y abajo) y 24 px de margen lateral. |

## 9. Íconos

Íconos de línea (estilo Lucide, trazo de 1.75 px a 16 px) dibujados con `ImDrawList`: no necesitan ninguna fuente de íconos y escalan con el DPI.

| `Icon_` | Uso |
|---|---|
| `Monitor` | Video |
| `Palette` | Gráficos |
| `Camera` | Cámara |
| `Volume` | Audio |
| `Gamepad` | Controles |
| `Accessibility` | Accesibilidad |
| `Info` | Acerca de |
| `Help` | Ayuda |
| `Folder` | Abrir archivo |
| `Refresh` | Restablecer, requiere reinicio |
| `Trash` | Borrar |

`void DrawIcon(ImDrawList* dl, Icon icon, ImVec2 pos, float size, ImU32 col)` dibuja un ícono en un cuadro de `size` px con esquina superior izquierda en `pos`. Úsalo con `M::iconSize * Dpi()` y un color de `Col()`.

## 10. Movimiento

Todas las animaciones duran `M::motion` (120 ms), siguen una curva de entrada y salida suaves (cúbica) y se calculan con `DeltaTime`, así que no dependen de los FPS. Empiezan en el mismo frame del clic y, si el destino cambia a mitad de camino, salen desde donde están:

- `ToggleSwitch`: la perilla y el color de la pista.
- `Segmented`: la pastilla se desliza entre opciones; el texto se recorta con su forma (oscuro dentro, gris fuera) y en «Personalizado» (`*v = -1`) se desvanece.
- `SidebarItem`: el fondo del ítem seleccionado se desliza a la nueva categoría.

No hace falta activar nada: los widgets guardan su estado de animación en el `ImGuiStorage` de la ventana.

La barra lateral dibuja en dos capas (`ImDrawList::ChannelsSplit`) entre `BeginSidebar()` y `EndSidebar()`: no uses `ChannelsSplit` propio dentro de ese bloque.

## 11. Demo

`void ShowDemoWindow(bool* open = nullptr)` en `imgui_moderno_demo.cpp`: ventana de ajustes de un emulador con todos los componentes, cambios pendientes, «Aplicar» condicional, Esc con confirmación, tema y escala al momento. Es la referencia de uso correcto.

## 12. Errores comunes

| Síntoma | Causa | Solución |
|---|---|---|
| Aviso «Code uses SetCursorPos() to extend window/parent boundaries» | `SettingRow()` o `SectionHeader()` sin nada debajo | Siempre un control tras `SettingRow` y filas tras `SectionHeader`. |
| Los colores vuelven a los anteriores al cambiar de tema | `ApplyTheme()` llamado entre un `PushStyleColor/Var` y su `Pop` | Llamarlo al principio del frame o antes de abrir ventanas. |
| Texto borroso o enorme al escalar | `ScaleAllSizes` llamado otra vez a mano | Solo `ApplyTheme(dark, dpi)`; no escalar dos veces. |
| La barra de acciones queda pegada al borde | Contenido creado con `BeginChild(…, -footerH)` propio | Usar `BeginSettingsContent()`, que ya reserva el espaciado. |
| El modal se cierra solo al abrirlo con Esc | Comprobación propia de Esc en el mismo frame | `ConfirmModal` ignora Esc en el frame en que aparece; no añadas otra. |
| El switch no queda a la derecha | Se usó fuera de `SettingRow` | Llama a `ImGui::SetNextItemWidth(ancho)` antes para definir la caja. |
