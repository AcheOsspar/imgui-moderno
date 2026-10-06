# SegmentedControl

Grupo de 2 a 4 opciones excluyentes visibles a la vez, como el perfil gráfico (Bajo, Medio, Alto, Ultra) o la frecuencia (30, 60, 90 Hz).

- Contenedor `bg-frame`, `radius-md`, 2 px de relleno. Segmento activo en `accent` con texto `on-accent`; el resto en `text-secondary`.
- Etiquetas cortas (una o dos palabras). Si no caben, usa Combo.
- Elegir un perfil cambia varias opciones; si el usuario toca una opción a mano, ningún segmento queda marcado y aparece la insignia neutra «Personalizado» junto a la etiqueta.

## Código

```cpp
static const char* const perfiles[] = { "Bajo", "Medio", "Alto", "Ultra" };
ImGuiModerno::RowOptions custom; custom.badge = "Personalizado"; custom.badgeTone = ImGuiModerno::Tone_Neutral;
ImGuiModerno::SettingRow("Perfil gráfico", "Ajusta varias opciones a la vez según la potencia de tu equipo.",
                         cfg.perfil < 0 ? custom : ImGuiModerno::RowOptions());
if (ImGuiModerno::Segmented("##perfil", &cfg.perfil, perfiles, IM_ARRAYSIZE(perfiles)))
    AplicarPerfil(cfg.perfil);

// Al tocar a mano una opción del perfil:
if (ImGuiModerno::ToggleSwitch("##contornos", &cfg.contornos)) cfg.perfil = -1;   // ningún segmento marcado
```

El contenedor mide 32 px (como cualquier control): 2 px de relleno y segmentos de 28 px.

Al elegir otra opción, la pastilla se desliza hasta ella en 120 ms (`M::motion`) y el texto se recorta con su forma, así se lee bien también durante el movimiento. En «Personalizado» la pastilla se desvanece.

Firmas completas en `docs/api.md`.

## Medidas

| Token | Valor | Escala | Dónde |
|---|---|---|---|
| `seg-padding` | 2px | — | Relleno del contenedor alrededor de los segmentos. |
| `seg-item-padding` | 0 12px | `space-3` | Relleno de cada segmento (alto 28 px). |
| `seg-item-radius` | 4px | `radius-sm` | Esquinas del segmento activo, dentro del contenedor de radius-md. |
