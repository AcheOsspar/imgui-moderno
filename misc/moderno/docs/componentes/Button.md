# Button

Botones de acción en cuatro variantes; como máximo un botón principal (`accent`) por vista.

| Variante | Uso | Fondo / texto |
|---|---|---|
| Principal | La acción que cierra la tarea: Aplicar, Iniciar juego | `accent` / `on-accent` |
| Secundario | Acciones normales: Elegir ROM, Probar sonido | `bg-frame` / `text-primary` |
| Fantasma | Acciones de bajo peso: Restablecer, Cancelar | transparente / `text-secondary` |
| Destructivo | Borrar o restablecer todo; siempre con confirmación | `danger-soft` / `danger` |

- 32 px de alto, relleno horizontal 16 px (`button-padding`), `radius-md`, etiqueta `label` (15 px 500).
- Verbo en infinitivo y sin signos: «Aplicar», «Elegir ROM». Nunca «OK» ni «Aceptar».
- Ícono opcional a la izquierda, 16 px.
- En la barra de acciones, el principal va a la derecha y el resto a su izquierda.

## Código

```cpp
if (ImGuiModerno::PrimaryButton("Aplicar"))       Apply();       // uno por vista
if (ImGuiModerno::SecondaryButton("Elegir ROM"))  PickRom();
if (ImGuiModerno::GhostButton("Restablecer"))     Reset();
if (ImGuiModerno::DangerButton("Borrar perfil"))  ImGui::OpenPopup("##borrar");   // siempre con ConfirmModal

// Grupo alineado a la derecha (barra de acciones)
const float gap = ImGuiModerno::M::space2 * ImGuiModerno::Dpi();
ImGuiModerno::SameLineRight(ImGuiModerno::ButtonWidth("Restablecer") + gap + ImGuiModerno::ButtonWidth("Aplicar"));
ImGuiModerno::GhostButton("Restablecer");
ImGui::SameLine(0, gap);
ImGuiModerno::PrimaryButton("Aplicar");
```

Ícono opcional a la izquierda, del set del sistema: `GhostButton("Ayuda", ImGuiModerno::Icon_Help)`, `DangerButton("Borrar perfil", ImGuiModerno::Icon_Trash)`. Si alineas a la derecha, pasa el mismo ícono a `ButtonWidth(label, icon)`.

Dentro de una fila (`SettingRow`), el botón se pega solo al borde derecho de la columna de control.

Firmas completas en `docs/api.md`.

## Medidas

| Token | Valor | Escala | Dónde |
|---|---|---|---|
| `button-padding` | 0 16px | `space-4` | Relleno horizontal (alto fijo 32 px). |
| `button-icon-gap` | 8px | `space-2` | Entre ícono y texto. |
| `button-group-gap` | 8px | `space-2` | Entre botones vecinos. |
