# Tooltip

Ayuda breve que aparece al pasar el cursor (o al enfocar con el mando) sobre un ícono de ayuda o una insignia.

- Fondo `bg-popup`, borde `border-subtle`, `radius-lg`, ancho máximo 280 px.
- Título opcional en `text-primary` 600; cuerpo en `caption` y `text-secondary`.
- Explica el efecto visible y el costo: «Dibuja cuadros intermedios… No cambia la velocidad del juego.»
- Retraso de 400 ms antes de mostrarse. Nunca pongas en un tooltip información imprescindible: esa va en la descripción de la fila.

## Código

```cpp
// En una fila: ícono «?» junto a la etiqueta
ImGuiModerno::RowOptions o;
o.helpTitle = "Interpolación visual";
o.helpBody  = "Dibuja cuadros intermedios para que el movimiento se vea más fluido.";
ImGuiModerno::SettingRow("Interpolación visual", "Movimiento más suave en juegos a 30 FPS.", o);

// Para cualquier ítem (título opcional)
ImGuiModerno::GhostButton("Ayuda");
ImGuiModerno::ItemTooltip(nullptr, "Abre la guía del emulador en el navegador.");

// Contenido propio con el relleno y ancho del sistema
if (ImGuiModerno::BeginItemTooltip()) {
    ImGui::TextUnformatted("…");
    ImGuiModerno::EndItemTooltip();
}
```

Firmas completas en `docs/api.md`.

## Medidas

| Token | Valor | Escala | Dónde |
|---|---|---|---|
| `tooltip-padding` | 8px 12px | `space-2 / space-3` | Relleno vertical / horizontal. |
| `tooltip-offset` | 8px | `space-2` | Distancia al elemento que lo abre. |
| `tooltip-text-gap` | 4px | `space-1` | Entre título y cuerpo. |
| `tooltip-max-width` | 280px | — | Ancho máximo antes de cortar líneas. |
