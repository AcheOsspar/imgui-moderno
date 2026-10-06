# ConfirmModal

Ventana modal que pide confirmación antes de una acción destructiva o de cerrar con cambios sin aplicar.

- Velo `bg-overlay` sobre todo; caja de 400 px, `bg-popup`, borde `border-subtle`, `radius-xl`, relleno `space-5`.
- Título como pregunta concreta que nombra el objeto: «¿Borrar el perfil «Alto 90 Hz»?».
- Cuerpo: qué se pierde y qué no. «Tus partidas guardadas no se verán afectadas.»
- Botones: Cancelar (fantasma) a la izquierda, acción (destructiva o principal) a la derecha, repitiendo el verbo del título.
- Esc y el botón B del mando cancelan. El foco inicial va en Cancelar.

## Código

```cpp
if (ImGuiModerno::DangerButton("Borrar perfil")) ImGui::OpenPopup("##borrar");

// Llamar cada frame, en la misma ventana donde se hizo OpenPopup
if (ImGuiModerno::ConfirmModal("##borrar", "¿Borrar el perfil «Alto 90 Hz»?",
        "Se eliminarán sus ajustes. Tus partidas guardadas no se verán afectadas.",
        "Borrar perfil" /*, destructive = true, cancelLabel = "Cancelar" */) == ImGuiModerno::ConfirmResult_Confirm)
    DeleteProfile();
```

Para confirmar algo no destructivo (p. ej. aplicar y reiniciar), pasa `destructive = false` y el botón será `PrimaryButton`.

Firmas completas en `docs/api.md`.

## Medidas

| Token | Valor | Escala | Dónde |
|---|---|---|---|
| `modal-width` | 400px | — | Ancho fijo de la caja. |
| `modal-padding` | 24px | `space-5` | Relleno de la caja. |
| `modal-title-gap` | 8px | `space-2` | Del título al cuerpo. |
| `modal-actions-gap` | 24px | `space-5` | Del cuerpo a los botones. |
| `modal-screen-margin` | 32px | `space-6` | Margen mínimo con el borde de la pantalla. |
