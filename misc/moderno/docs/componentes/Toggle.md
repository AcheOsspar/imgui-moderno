# Toggle

Interruptor de encendido y apagado para opciones binarias que se aplican al instante; reemplaza al checkbox por defecto de ImGui.

- Pista de 36 × 20 px, `radius-full`. Encendido: `accent` con perilla `knob` a la derecha. Apagado: `bg-frame-active` con perilla `knob-off` a la izquierda.
- Usa un checkbox solo dentro de listas de selección múltiple (por ejemplo, qué capas de texturas cargar).
- El cambio se ve en el juego de inmediato. Si requiere reinicio, añade la insignia correspondiente en la fila.

## Código

```cpp
ImGuiModerno::SettingRow("Sincronización vertical", "Evita cortes en la imagen. Puede añadir un poco de retraso.");
if (ImGuiModerno::ToggleSwitch("##vsync", &cfg.vsync))
    AplicarVsync(cfg.vsync);   // devuelve true en el frame en que cambia
```

Ocupa el alto de un control (32 px) con la pista de 20 px centrada. En una fila se pega al borde derecho; fuera de ella, llama antes a `ImGui::SetNextItemWidth(ancho)` si quieres alinearlo a la derecha de una caja.

Firmas completas en `docs/api.md`.

## Medidas

| Token | Valor | Escala | Dónde |
|---|---|---|---|
| `toggle-size` | 36px 20px | — | Ancho × alto de la pista. |
| `toggle-knob` | 14px | — | Diámetro de la perilla. |
| `toggle-inset` | 3px | — | Distancia de la perilla al borde de la pista. |
