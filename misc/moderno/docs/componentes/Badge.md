# Badge

Etiqueta pequeña junto al nombre de un ajuste para avisar de una consecuencia: requiere reinicio, es exigente, está verificada o es experimental.

- 20 px de alto, `radius-sm`, texto 12 px 500, ícono opcional de 12 px.
- Advertencia (`warning-soft` / `warning`): «Requiere reinicio», «Exigente».
- Correcto (`success-soft` / `success`): «Verificada».
- Neutro (`bg-frame` / `text-secondary`): «Beta», «Experimental».
- Una insignia por fila como máximo. El detalle va en un Tooltip al pasar el cursor.

## Código

```cpp
// En una fila (lo habitual): la insignia va junto a la etiqueta
ImGuiModerno::RowOptions o;
o.badge = "Requiere reinicio";               // badgeTone = Tone_Warning por defecto
ImGuiModerno::SettingRow("Shaders asíncronos", "Reduce tirones la primera vez que aparece un efecto.", o);
ImGuiModerno::ToggleSwitch("##async", &cfg.asyncShaders);

// Suelta: en la columna de control se alinea a la derecha y se centra
ImGuiModerno::Badge("Verificada", ImGuiModerno::Tone_Success);
ImGuiModerno::Badge("Beta");                 // Tone_Neutral
```

Firmas completas en `docs/api.md`.

## Medidas

| Token | Valor | Escala | Dónde |
|---|---|---|---|
| `badge-padding` | 0 8px | `space-2` | Relleno horizontal (alto fijo 20 px). |
| `badge-icon-gap` | 4px | `space-1` | Entre ícono de 12 px y texto. |
