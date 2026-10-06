# Slider

Control deslizante fino para valores continuos (campo de visión, volumen, intensidad), con la lectura del valor a la derecha.

- Pista de 4 px (`bg-frame-active`), tramo recorrido en `accent`, perilla circular de 14 px en `accent` con un anillo de 2 px del color de fondo.
- Valor en `value` (mono 13 px, `text-secondary`) con su unidad: «1.10x», «30 %», «90 Hz».
- Clic derecho o doble clic vuelve al valor por defecto.
- Si solo hay 3 o 4 valores posibles, usa SegmentedControl en vez de slider.

## Código

```cpp
ImGuiModerno::SettingRow("Campo de visión", "Amplía lo que ves a los lados. Clic derecho para volver a 1.00x.");
ImGuiModerno::SliderFloat("##fov", &cfg.fov, 0.8f, 1.5f, "%.2fx", /*reset=*/1.0f, /*step=*/0.01f);

ImGuiModerno::SettingRow("Volumen general");
ImGuiModerno::SliderFloat("##vol", &cfg.vol, 0, 100, "%.0f %%", /*reset=*/80.0f, /*step=*/1.0f);

ImGuiModerno::SettingRow("Frecuencia");
ImGuiModerno::SliderInt("##hz", &cfg.hz, 30, 144, "%d Hz", /*reset=*/60);
```

Con el foco (teclado o mando), izquierda y derecha mueven el valor un `step` (por defecto, 1 % del rango).

Firmas completas en `docs/api.md`.

## Medidas

| Token | Valor | Escala | Dónde |
|---|---|---|---|
| `slider-hit-height` | 32px | — | Área táctil vertical; igual al alto de un control. |
| `slider-track` | 4px | `space-1` | Grosor de la pista. |
| `slider-knob` | 14px | — | Diámetro de la perilla. |
| `slider-value-gap` | 12px | `space-3` | Entre la pista y la lectura del valor. |
| `slider-value-width` | 44px | — | Ancho mínimo de la lectura, alineada a la derecha. |
