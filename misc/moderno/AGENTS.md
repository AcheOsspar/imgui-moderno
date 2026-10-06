# ImGui Moderno: instrucciones para asistentes de código

Este archivo es para cualquier asistente de IA (Claude Code, Codex, Cursor, Copilot…) o persona que vaya a **crear, modificar o migrar una interfaz de Dear ImGui** con el sistema ImGui Moderno. Contiene todo lo necesario para aplicarlo bien; los documentos de `docs/` amplían cada punto.

Esta carpeta es la fuente de verdad del diseño. Si algo de aquí choca con una costumbre de ImGui, gana este archivo.

## 0. Antes de tocar código

1. Lee este archivo completo.
2. Consulta `docs/api.md` para las firmas exactas: **no inventes funciones ni parámetros**.
3. Para cada componente que uses, mira su ficha en `docs/componentes/` (reglas de uso y de texto).
4. Para medidas que no estén cubiertas por un widget, usa `docs/medidas.md`; nunca valores fuera de la escala.
5. Usa `imgui_moderno_demo.cpp` como ejemplo de referencia de una ventana completa y correcta.

Requisitos: Dear ImGui **1.92 o posterior** (fuentes dinámicas `PushFont(font, size)`), C++11.

## 1. Reglas obligatorias

1. **Tema**: `ImGuiModerno::ApplyTheme(dark, dpi)` una vez al iniciar y al cambiar modo o escala. Nunca `ImGui::StyleColorsDark/Light/Classic()` ni `style.ScaleAllSizes()` aparte (ya lo hace `ApplyTheme`). Llámalo fuera de cualquier `Push`/`Pop`.
2. **Color**: todo color sale de `ImGuiModerno::Col()` (`textSecondary`, `accent`, `danger`…). Nunca `ImVec4(…)` ni `IM_COL32(…)` sueltos en la interfaz.
3. **Acento Grafito (monocromo)**: no introduzcas azules ni otros acentos. El color vivo se reserva para estados: `success`, `warning`, `danger`.
4. **Medidas**: solo la escala 4, 8, 12, 16, 24, 32 (`M::space1`…`M::space6`) y las alturas fijas (20 insignia, 28 segmento, 32 control/botón, 36 navegación, 44 fila mínima, 56 barra de acciones). Excepciones permitidas: 1 px líneas, 2 px entre ítems de navegación y dentro del segmentado, 3 px perilla del switch. **Multiplica toda medida propia por `ImGuiModerno::Dpi()`**.
5. **Tipografía**: solo sans-serif. Inter para la interfaz, JetBrains Mono para valores y códigos. Cambia de estilo con `PushTextStyle(TextStyle_…)` / `PopTextStyle()`, nunca con `PushFont` a mano.
6. **Cada ajuste es una fila**: `BeginSettingRows` → `SettingRow(etiqueta, descripción)` → **un** control → … → `EndSettingRows`. Etiqueta y descripción a la izquierda; control a la derecha en la columna de 240 px.
7. **Control según el tipo de opción** (tabla de la sección 3). Sí/no con `ToggleSwitch`, **nunca** `ImGui::Checkbox`.
8. **Un solo `PrimaryButton` por vista.** Acciones destructivas con `DangerButton` **y** `ConfirmModal`.
9. **Toda opción deshabilitada explica el motivo** en su descripción.
10. **Textos**: ver sección 5.
11. **`tokens.json` e `imgui_moderno.h` deben coincidir.** Si cambias un valor en uno, cámbialo en el otro y en `docs/medidas.md` (sección 8).

## 2. Integración en un proyecto

```cpp
#include "imgui_moderno.h"

// Al iniciar, después de ImGui::CreateContext() y de iniciar los backends:
ImGuiModerno::LoadFonts("fonts/Inter-Regular.ttf", "fonts/Inter-Medium.ttf",
                        "fonts/Inter-SemiBold.ttf", "fonts/JetBrainsMono-Regular.ttf"); // rutas inexistentes se ignoran
ImGuiModerno::ApplyTheme(/*dark=*/true, /*dpi=*/main_scale);                           // en lugar de StyleColorsDark()
```

- Añade `imgui_moderno_demo.cpp` al build solo si quieres `ShowDemoWindow()`.
- Activa la navegación: `io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;`
- Modo oscuro por defecto sobre el juego; claro para instaladores o herramientas de escritorio.
- Si el usuario cambia tema o escala en el propio menú, aplica el cambio al comienzo del frame siguiente o antes de abrir ventanas (como hace la demo).

## 3. Qué control usar

| Tipo de opción | Usa | No uses |
|---|---|---|
| Sí / no | `ToggleSwitch` | `ImGui::Checkbox` (solo en listas de selección múltiple) |
| 2 a 4 opciones cortas | `Segmented` | `RadioButton`, `Combo` |
| 5 o más opciones | `Combo` | `ImGui::Combo`, `ListBox` |
| Valor continuo | `SliderFloat` / `SliderInt` con unidad en el formato y valor por defecto en `reset` | `ImGui::SliderFloat`, `DragFloat` |
| Acción que cierra la tarea | `PrimaryButton` (uno por vista) | |
| Acción normal | `SecondaryButton` | |
| Acción de poco peso | `GhostButton` | |
| Borrar o restablecer todo | `DangerButton` + `ConfirmModal` | Borrar sin confirmar |
| Consecuencia (reinicio, costo, beta) | `RowOptions::badge` en la fila | Texto entre paréntesis en la etiqueta |
| Explicación extra no imprescindible | `RowOptions::helpTitle/helpBody` | Información obligatoria en un tooltip |
| Resultado, error o aviso | `Notice` | `TextColored` rojo |
| Lectura de un valor | `Value` | `ImGui::Text` |
| Selector de idioma | `Segmented` con `LanguageName()` + `SetLanguage()` | `Combo` con códigos («es», «en») |
| Grupo de filas | `SectionHeader` | `CollapsingHeader`, `SeparatorText` |
| Opciones avanzadas | `CollapsibleSection("Avanzado")`, al final | |
| Categorías | `BeginSettingsWindow` + `BeginSidebar`/`SidebarItem` con su `Icon_*` | `BeginTabBar`, ventanas sueltas |
| Ícono en botón o navegación | `Icon_*` del sistema (`Icon_Help`, `Icon_Trash`…) | Emojis, PNG, íconos rellenos o a color |

## 4. Organización de la información

- **Una sola ventana de ajustes**: barra lateral (categorías), contenido con scroll y barra de acciones. Cambiar de categoría cambia el contenido, nunca abre otra ventana.
- **Orden de categorías**: Video, Gráficos, Cámara, Audio, Controles, Accesibilidad, separador, Acerca de. Omite las que no apliquen; no inventes otras sin necesidad.
- **Cuatro niveles como máximo**: categoría → sección → ajuste → ajuste dependiente (`RowOptions::dependent`, sangría de 24 px, deshabilitado u oculto si la madre está apagada).
- **Dentro de una categoría**: perfil o preset arriba; luego lo que más se cambia; lo exigente más abajo con su insignia; **Avanzado** siempre al final y colapsado.
- **5 o 6 filas por sección** como máximo; si hay más, divide.
- **Aplicación de cambios**: por defecto al instante. «Aplicar» solo aparece si hay cambios pendientes (p. ej. los que requieren reinicio). Esc cierra; con cambios sin aplicar, pregunta con `ConfirmModal`.

## 5. Textos

El sistema admite **español, inglés y portugués (Brasil)**. Todo texto visible pasa por `Tr("es", "en", "pt")` (o por el sistema de traducción que ya tenga el proyecto) y se escribe en los tres idiomas a la vez; nunca dejes un idioma vacío ni copies el español en los otros. Si el proyecto solo usa un idioma, respeta ese y aplica las mismas reglas de estilo.

Reglas por idioma:

| | Español | English | Português (Brasil) |
|---|---|---|---|
| Trato | Tuteo, español neutro: «Elige», «Puedes» | Segunda persona directa: «Choose», «You can» | «Você»: «Escolha», «Você pode» |
| Mayúsculas | Solo la inicial: «Campo de visión» | *Sentence case*, no *Title Case*: «Field of view» | Solo la inicial: «Campo de visão» |
| Botones | Infinitivo: «Aplicar», «Elegir ROM» | Imperativo: «Apply», «Choose ROM» | Infinitivo: «Aplicar», «Escolher ROM» |
| Vocabulario de juego | mando, pantalla, ajustes, partida guardada | controller, screen, settings, saved game | controle, tela, configurações, jogo salvo |
| Nunca | «OK», «Aceptar» | «OK», «Submit» | «OK», «Ok» |

Longitud: el portugués suele ser hasta un 30 % más largo que el inglés y el español un 20 %. Comprueba que las etiquetas de `Segmented` (2 a 4 opciones en 240 px) caben en los tres idiomas; si no, acórtalas o usa `Combo`.

IDs: si una etiqueta traducida es también el ID de algo con estado (secciones colapsables, ítems de navegación en bucle), fíjalo con `###id` o `PushID(i)` para que no cambie con el idioma (ver `docs/api.md`, Idiomas).

Ejemplos en español (las reglas son las mismas en los tres idiomas):

| Elemento | Regla | Bien | Mal |
|---|---|---|---|
| Etiqueta | Sustantivo corto, mayúscula inicial, sin dos puntos | «Campo de visión» | «Activar FOV:» |
| Descripción | Una línea, dice qué cambia en pantalla | «Bordes oscuros en personajes y objetos.» | «Outline pass enabled» |
| Botón | Verbo en infinitivo, sin signos | «Aplicar», «Elegir ROM» | «OK», «Aceptar», «¡Listo!» |
| Opción deshabilitada | Explica el motivo | «No disponible: necesita una GPU compatible con Vulkan 1.3.» | (deshabilitada sin texto) |
| Error (`Notice`) | Título qué pasó + cuerpo qué hacer + código | «No se detecta ningún mando» / «Conecta un mando…» / «Código CTL-0042» | «Error 0x8007» solo |
| Modal | Pregunta concreta que nombra el objeto; botón repite el verbo | «¿Borrar el perfil «Alto 90 Hz»?» → «Borrar perfil» | «¿Estás seguro?» → «Sí» |
| Opciones de combo | Dicen el resultado | «x2 · 800×480» | «2» |
| Valores | Con unidad | «90 Hz», «30 %», «1.10x» | «90», «0.3» |

Sin signos de exclamación en mensajes del sistema. Los estados nunca se comunican solo con color: siempre ícono y texto.

## 6. Cómo migrar una interfaz existente (paso a paso)

1. **Localiza** el código de la interfaz: busca `ImGui::Begin(`, `Checkbox(`, `SliderFloat(`, `Combo(`, `StyleColors`, `PushStyleColor`.
2. **Tema**: sustituye `StyleColors*()`, `ScaleAllSizes()` y colores propios por `LoadFonts()` + `ApplyTheme()`. Borra los `PushStyleColor`/`PushStyleVar` decorativos; los widgets del sistema ya aplican sus estilos.
3. **Estructura**: agrupa las opciones en categorías (sección 4). Si hoy hay ventanas o pestañas sueltas, conviértelas en una `SettingsWindow` con barra lateral.
4. **Filas**: cada `ImGui::Text("X:") + widget` o widget con etiqueta incorporada pasa a `SettingRow("X", "qué cambia")` + control con id oculto (`"##x"`).
5. **Controles**: cámbialos según la tabla de la sección 3. Conserva las mismas variables y la misma lógica: solo cambia la presentación.
6. **Textos**: reescríbelos según la sección 5. Añade descripción a cada ajuste que no se explique solo.
7. **Estados especiales**: dependientes con `dependent`, deshabilitados con motivo, consecuencias con `badge`, destructivos con `ConfirmModal`.
8. **Verifica** con la lista de la sección 9 y, si puedes, compila con `-Wall -Wextra` y ejecuta.

Antes / después:

```cpp
// Antes
ImGui::Checkbox("Enable outlines", &cfg.outlines);
ImGui::SliderFloat("FOV", &cfg.fov, 0.8f, 1.5f);
if (ImGui::Button("Reset all")) ResetAll();

// Después
if (ImGuiModerno::BeginSettingRows("##estilo")) {
    ImGuiModerno::SettingRow("Contornos", "Bordes oscuros en personajes y objetos.");
    ImGuiModerno::ToggleSwitch("##contornos", &cfg.outlines);
    ImGuiModerno::SettingRow("Campo de visión", "Amplía lo que ves a los lados. Clic derecho para volver a 1.00x.");
    ImGuiModerno::SliderFloat("##fov", &cfg.fov, 0.8f, 1.5f, "%.2fx", /*reset=*/1.0f, /*step=*/0.01f);
    ImGuiModerno::SettingRow("Restablecer todo", "Vuelve a la configuración de fábrica. Tus partidas guardadas no se borran.");
    if (ImGuiModerno::DangerButton("Restablecer todo")) ImGui::OpenPopup("##reset");
    ImGuiModerno::EndSettingRows();
}
if (ImGuiModerno::ConfirmModal("##reset", "¿Restablecer todos los ajustes?",
        "Se perderá tu configuración. Tus partidas guardadas no se verán afectadas.", "Restablecer todo")
    == ImGuiModerno::ConfirmResult_Confirm)
    ResetAll();
```

## 7. Errores que debes evitar

- Dejar un `SettingRow()` sin control debajo, o un `SectionHeader()` sin filas: ImGui avisa de que el cursor se movió fuera del contenido.
- Llamar a `ApplyTheme()` en medio de un `Push`/`Pop`: al hacer `Pop` vuelven los valores viejos.
- Crear tu propio `BeginChild(…, -footer)` para el contenido: usa `BeginSettingsContent()`, que reserva también el `ItemSpacing.y` (si no, la barra de acciones queda recortada y pegada al borde).
- Llamar a `ConfirmModal()` solo cuando se abre: hay que llamarlo cada frame, en la misma ventana donde se hizo `OpenPopup`.
- Poner dos `PrimaryButton` en la misma vista, o un `DangerButton` sin confirmación.
- Usar `ImGui::TextWrapped` con un `PushTextWrapPos` propio: `TextWrapped` lo ignora; usa `TextUnformatted` con el wrap activo.
- Escribir medidas sin `* Dpi()` o colores sin `Col()`.
- Añadir animaciones propias con otra duración: todo movimiento usa `M::motion` (120 ms) y solo para cambios de posición o estado.

## 8. Si modificas el sistema

Cualquier cambio en `imgui_moderno.h` se refleja en **todos** estos sitios en el mismo cambio:

| Si cambias | Actualiza también |
|---|---|
| Un color de `Dark()` / `Light()` | `tokens.json` → `color.tokens` |
| Una constante de `M::` | `tokens.json` → `layout`, `spacing`, `radius` o `medidas`; `docs/medidas.md`; la ficha del componente |
| Un valor de `ApplyTheme()` | `tokens.json` → `imgui.tokens`; `docs/tema-imgui.md` |
| Una función pública o su firma | `docs/api.md`; la ficha del componente; `imgui_moderno_demo.cpp` si la usa |
| Una regla de uso | este archivo (`AGENTS.md`) y `docs/guia.md` |

Después de cualquier cambio ejecuta `python misc/moderno/tools/check_tokens.py` (o `python tools/check_tokens.py` desde esta carpeta): compara colores, medidas y tamaños de texto entre `tokens.json` y el header, y falla si no coinciden.

Un componente nuevo: usa una de las alturas fijas, solo valores de la escala, colores de `Col()`, añade su ficha en `docs/componentes/`, su sección en `docs/api.md` y úsalo en la demo.

## 9. Lista de verificación final

- [ ] `ApplyTheme()` se llama al iniciar y al cambiar modo o escala; no queda ningún `StyleColors*()`.
- [ ] No hay colores ni medidas sueltos: todo sale de `Col()` y `M::* × Dpi()`.
- [ ] Cada ajuste es un `SettingRow` con descripción, seguido de un control.
- [ ] Los sí/no son `ToggleSwitch`; 2–4 opciones `Segmented`; 5+ `Combo`; continuos `SliderFloat/Int` con unidad.
- [ ] Hay como máximo un `PrimaryButton` por vista y cada `DangerButton` abre un `ConfirmModal`.
- [ ] Toda opción deshabilitada explica el motivo.
- [ ] Textos: etiquetas sin dos puntos, botones en infinitivo (imperativo en inglés), errores con qué pasó + qué hacer + código.
- [ ] Cada texto visible está en español, inglés y portugués, y las etiquetas cortas caben en los tres.
- [ ] «Avanzado» es la última sección y está colapsada.
- [ ] Se puede recorrer todo con teclado y mando.
- [ ] Compila sin advertencias y no aparece ningún aviso de ImGui en pantalla.
- [ ] Si tocaste el sistema: `tools/check_tokens.py` pasa y `docs/api.md` refleja las firmas.
