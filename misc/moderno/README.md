# ImGui Moderno

Sistema de diseño para interfaces hechas con Dear ImGui: un tema (paleta Grafito, modo oscuro y claro) y widgets listos para armar menús de ajustes claros y amigables, pensados para emuladores, juegos y herramientas.

*A design system for Dear ImGui: a monochrome theme (dark and light) plus ready-made widgets for friendly settings menus. Header-only; drop `imgui_moderno.h` into your project.*

## Qué incluye

| Archivo | Contenido |
|---|---|
| `imgui_moderno.h` | Tema y widgets (header-only, Dear ImGui 1.92 o posterior) |
| `imgui_moderno_demo.cpp` | `ImGuiModerno::ShowDemoWindow()`: ventana de ajustes de un emulador con todos los componentes |
| `tokens.json` | Todos los valores del sistema (color, tipografía, espaciado, medidas) |
| `AGENTS.md` | Instrucciones completas para aplicarlo o migrar una interfaz, pensadas para asistentes de IA y para personas |
| `docs/api.md` | Referencia de cada función, con ejemplos y errores comunes |
| `docs/` | Guía de diseño, medidas, tema y una ficha por componente |
| `tools/check_tokens.py` | Verifica que `tokens.json` y el header coinciden |

Componentes: `ToggleSwitch`, `Segmented`, `Combo`, `SliderFloat` / `SliderInt`, `PrimaryButton` / `SecondaryButton` / `GhostButton` / `DangerButton`, `Badge`, `Value`, `Notice`, `HelpMarker` / `ItemTooltip`, `SectionHeader`, `CollapsibleSection`, `SettingRow`, `ConfirmModal`, y la ventana completa con `BeginSettingsWindow`, `BeginSidebar` / `SidebarItem` y `BeginSettingsContent` / `BeginSettingsFooter`.

## Verlo en acción

Ejemplo para Windows + DirectX 11 en `examples/example_moderno_win32_directx11/`: abre la ventana de ajustes sobre una pantalla de juego simulada. Pulsa F1 para mostrarla u ocultarla.

Con Visual Studio, desde una *Developer Command Prompt*:

```bat
cd examples\example_moderno_win32_directx11
build_win32.bat
Debug\example_moderno_win32_directx11.exe
```

## Cómo usarlo en tu proyecto

1. Copia `imgui_moderno.h` (y, si quieres la demo, `imgui_moderno_demo.cpp`) junto a tus archivos de Dear ImGui.
2. Al iniciar, después de crear el contexto:

   ```cpp
   #include "imgui_moderno.h"

   // Opcional: Inter y JetBrains Mono. Las rutas que no existan se ignoran.
   ImGuiModerno::LoadFonts("fonts/Inter-Regular.ttf", "fonts/Inter-Medium.ttf",
                           "fonts/Inter-SemiBold.ttf", "fonts/JetBrainsMono-Regular.ttf");
   ImGuiModerno::ApplyTheme(/*dark=*/true, /*dpi=*/main_scale);   // en lugar de ImGui::StyleColorsDark()
   ```

3. Arma tus ajustes con filas:

   ```cpp
   if (ImGuiModerno::BeginSettingRows("##graficos")) {
       ImGuiModerno::SettingRow("Contornos", "Bordes oscuros en personajes y objetos.");
       ImGuiModerno::ToggleSwitch("##contornos", &cfg.outlines);

       ImGuiModerno::RowOptions exigente; exigente.badge = "Exigente";
       ImGuiModerno::SettingRow("Resolución interna", "Dibuja el juego a más resolución.", exigente);
       ImGuiModerno::Combo("##res", &cfg.resolution, resoluciones, IM_ARRAYSIZE(resoluciones));

       ImGuiModerno::SettingRow("Campo de visión");
       ImGuiModerno::SliderFloat("##fov", &cfg.fov, 0.8f, 1.5f, "%.2fx", /*reset=*/1.0f);
       ImGuiModerno::EndSettingRows();
   }
   ```

   `SettingRow()` deja el cursor en la columna derecha (240 px); el control que pongas a continuación se alinea al borde derecho y se centra con el texto.

4. Para una ventana de ajustes completa (barra lateral, contenido con scroll y barra de acciones), copia la estructura de `ShowDemoWindow()` en `imgui_moderno_demo.cpp`.

Si el tema o la escala cambian mientras corre el programa, vuelve a llamar a `ApplyTheme()` fuera de cualquier `Push`/`Pop` de estilo.

## Licencia

MIT, © 2026 AcheOsspar (ver `LICENSE`). Hecho sobre Dear ImGui, © Omar Cornut, también MIT. Si lo incluyes en tu proyecto, conserva los dos avisos de copyright.

## Fuentes

El sistema usa Inter y JetBrains Mono (licencia SIL OFL, no se incluyen aquí). Sin ellas funciona con la fuente por defecto de Dear ImGui; el ejemplo de Windows usa Segoe UI y Consolas como respaldo.

## Con asistentes de IA

`AGENTS.md` reúne todo lo que un asistente necesita para aplicar el sistema correctamente: reglas obligatorias, qué control usar, cómo escribir los textos, cómo migrar una interfaz paso a paso, errores a evitar y la lista de verificación. Al copiar la carpeta a tu proyecto, apunta tu asistente a ese archivo:

| Asistente | Qué hacer |
|---|---|
| Claude Code | En el `CLAUDE.md` de la raíz: `@ruta/a/moderno/CLAUDE.md` (que a su vez importa `AGENTS.md`) |
| Codex, Jules y otros que leen `AGENTS.md` | En el `AGENTS.md` de la raíz: «Para toda interfaz de Dear ImGui sigue `ruta/a/moderno/AGENTS.md`» |
| Cursor | Una regla en `.cursor/rules/` que diga lo mismo, o añade `AGENTS.md` como contexto |
| GitHub Copilot | En `.github/copilot-instructions.md`: «Para toda interfaz de Dear ImGui sigue `ruta/a/moderno/AGENTS.md`» |

Luego basta con pedir, por ejemplo: «Aplica ImGui Moderno al menú de ajustes» o «Migra la ventana de opciones de video a ImGui Moderno».
