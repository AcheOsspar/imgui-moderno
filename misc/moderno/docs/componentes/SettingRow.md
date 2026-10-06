# SettingRow

El patrón central del sistema: cada ajuste es una fila con etiqueta y descripción a la izquierda y el control alineado a la derecha.

## Código

```cpp
if (ImGuiModerno::BeginSettingRows("##estilo")) {
    // Fila simple
    ImGuiModerno::SettingRow("Contornos", "Bordes oscuros en personajes y objetos.");
    ImGuiModerno::ToggleSwitch("##contornos", &cfg.outlines);

    // Con insignia y ayuda
    ImGuiModerno::RowOptions o;
    o.badge = "Exigente";
    o.helpTitle = "Interpolación visual";
    o.helpBody = "Dibuja cuadros intermedios para que el movimiento se vea más fluido. No cambia la velocidad del juego.";
    ImGuiModerno::SettingRow("Interpolación visual", "Movimiento más suave en juegos a 30 FPS.", o);
    ImGuiModerno::ToggleSwitch("##interp", &cfg.interp);

    // Dependiente y deshabilitada con motivo
    ImGuiModerno::RowOptions dep; dep.dependent = true;
    ImGui::BeginDisabled(!cfg.interp);
    ImGuiModerno::SettingRow("Cuadros intermedios",
        cfg.interp ? "Más cuadros, más fluidez y más consumo." : "Activa Interpolación visual para elegir cuántos.", dep);
    ImGuiModerno::Segmented("##frames", &cfg.frames, frames, 2);
    ImGui::EndDisabled();

    ImGuiModerno::EndSettingRows();
}
```

`SettingRow` centra el bloque de texto con el control (32 px) y deja el cursor en la columna derecha con `SetNextItemWidth(-FLT_MIN)`. Siempre va seguida de **un** control.

Firmas completas en `docs/api.md`.

## Medidas

| Token | Valor | Escala | Dónde |
|---|---|---|---|
| `row-padding` | 12px 0 | `space-3` | Relleno vertical de la fila (alto mínimo 44 px). |
| `row-gap` | 24px | `space-5` | Mínimo entre el bloque de texto y la columna del control. |
| `row-text-gap` | 4px | `space-1` | Entre etiqueta y descripción. |
| `row-label-badge-gap` | 8px | `space-2` | Entre la etiqueta y su insignia o ícono de ayuda. |
| `row-indent` | 24px | `space-5` | Sangría de una opción dependiente. |

## Reglas

- **Etiqueta** (`label`, 15 px 500): nombre corto, sin dos puntos. «Contornos», no «Activar contornos:».
- **Descripción** (`caption`, 13 px, `text-secondary`): una línea que dice qué cambia en pantalla. Opcional cuando la etiqueta lo explica sola.
- **Control** en una columna fija de `control-width` (240 px), alineada a la derecha. Todos los controles de la ventana comparten ese borde.
- Alto mínimo `row-min-height` (44 px), relleno vertical `space-3`, línea `separator` entre filas.
- **Opción dependiente** (solo tiene sentido si otra está activa): sangría de 24 px (`row-indent`) debajo de su opción madre; se oculta o se deshabilita cuando la madre está apagada.
- **Deshabilitada**: texto en `text-disabled` y la descripción explica por qué («Necesita una GPU compatible con…»). Nunca deshabilitar sin explicar.
- Insignias (Badge) junto a la etiqueta para avisos como «Requiere reinicio» o «Exigente».
