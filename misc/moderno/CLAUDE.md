# Sistema de diseño ImGui Moderno

Esta carpeta es la fuente de verdad para toda interfaz hecha con Dear ImGui en este proyecto. Las reglas completas, cómo integrarlo, qué control usar, cómo escribir los textos, cómo migrar una interfaz y la lista de verificación están en `AGENTS.md` (compartido con otros asistentes):

@AGENTS.md

## Archivos

| Archivo | Contenido |
|---|---|
| `AGENTS.md` | Instrucciones completas para aplicar el sistema (este archivo las importa) |
| `imgui_moderno.h` | Tema y widgets (header-only, Dear ImGui 1.92+) |
| `imgui_moderno_demo.cpp` | `ShowDemoWindow()`: ventana de ajustes de emulador con todos los componentes; referencia de uso |
| `tokens.json` | Todos los tokens: color (oscuro y claro), tipografía, espaciado, radios, layout, `ImGuiStyle` y medidas por componente |
| `docs/api.md` | Referencia de cada función, con ejemplos y errores comunes |
| `docs/guia.md` | Guía de diseño: principios, layout, organización, tipografía, color, íconos, escritura |
| `docs/medidas.md` | Padding, espaciado y gap de cada componente |
| `docs/tema-imgui.md` | Cómo se mapean los tokens a `ImGuiStyle` y cómo se cargan las fuentes |
| `docs/componentes/*.md` | Ficha de cada uno de los componentes |
