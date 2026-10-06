# SettingsWindow

La ventana de ajustes completa: barra lateral de categorías a la izquierda, contenido con scroll al centro y barra de acciones abajo.

## Código

```cpp
if (ImGuiModerno::BeginSettingsWindow("Ajustes##app", &open)) {
    ImGuiModerno::BeginSidebar("Ajustes");
    /* SidebarItem por categoría */
    ImGuiModerno::EndSidebar();

    if (ImGuiModerno::BeginSettingsContent(nombres[cat])) DibujarCategoria(cat);
    ImGuiModerno::EndSettingsContent();                    // siempre

    ImGuiModerno::BeginSettingsFooter();
    ImGuiModerno::GhostButton("Ayuda");
    const float gap = ImGuiModerno::M::space2 * ImGuiModerno::Dpi();
    ImGuiModerno::SameLineRight(ImGuiModerno::ButtonWidth("Restablecer") + (pendientes ? gap + ImGuiModerno::ButtonWidth("Aplicar") : 0));
    if (ImGuiModerno::GhostButton("Restablecer")) Restablecer();
    if (pendientes) { ImGui::SameLine(0, gap); if (ImGuiModerno::PrimaryButton("Aplicar")) Aplicar(); }
    ImGuiModerno::EndSettingsFooter();
}
ImGuiModerno::EndSettingsWindow();                         // siempre, como ImGui::End()
```

`BeginSettingsContent` reserva la barra de acciones (56 px) **más** el `ItemSpacing.y` que ImGui añade tras el contenido; así la barra queda entera y sus botones centrados (12 px arriba y abajo). No sustituyas el contenido por un `BeginChild` propio. La ventana completa de ejemplo, con cambios pendientes y Esc con confirmación, está en `imgui_moderno_demo.cpp`.

Firmas completas en `docs/api.md`.

## Medidas

| Token | Valor | Escala | Dónde |
|---|---|---|---|
| `window-padding` | 0 | — | La ventana no tiene relleno propio; lo ponen la barra lateral y el contenido. |
| `window-radius` | 12px | `radius-xl` | Esquinas de la ventana. |
| `content-padding` | 24px | `space-5` | Relleno del área de contenido en los cuatro lados. |
| `page-title-gap` | 16px | `space-4` | Del título de página a la primera sección. |
| `footer-padding` | 0 24px | `space-5` | Relleno horizontal de la barra de acciones (alto fijo 56 px). |
| `footer-gap` | 8px | `space-2` | Entre botones de la barra de acciones. |

## Anatomía

| Zona | Medida | Contenido |
|---|---|---|
| Ventana | `window-width` × `window-height` (880 × 600), centrada | Sin borde, `radius-xl`, fondo `bg-window` |
| Barra lateral | `sidebar-width` (200), alto completo | Título «Ajustes» + categorías (ver SidebarNav) |
| Contenido | El resto, relleno `space-5` | Título de página + secciones + filas |
| Barra de acciones | `footer-height` (56) | Ayuda a la izquierda; Restablecer y Aplicar a la derecha |

## Reglas

- Una sola ventana a la vez. Abrir otra categoría cambia el contenido, nunca abre otra ventana.
- El contenido es lo único que hace scroll; la barra lateral y la de acciones quedan fijas.
- Si todos los cambios se aplican al instante, la barra de acciones solo muestra la ayuda y **Restablecer**. **Aplicar** aparece únicamente cuando hay cambios pendientes.
- Esc cierra la ventana. Si hay cambios sin aplicar, pregunta antes (ver ConfirmModal).
