# Medidas y espaciado

Reglas de padding, espaciado y gap que todo componente nuevo o existente debe respetar. Los valores están a escala 1 (1080p); en ImGui multiplícalos por el factor DPI.

## Reglas generales

1. **Escala de 4.** Usa solo `space-1` (4), `space-2` (8), `space-3` (12), `space-4` (16), `space-5` (24) y `space-6` (32). Excepciones permitidas: 1 px para líneas, 2 px entre ítems de navegación y dentro del selector segmentado, 3 px para la perilla del switch.
2. **Padding según el tamaño del contenedor.**

| Contenedor | Padding |
|---|---|
| Insignia | 0 × 8 (`space-2`) |
| Controles y botones (32 px de alto) | 0 × 12 a 16 (`space-3` / `space-4`) |
| Tooltip | 8 × 12 (`space-2` × `space-3`) |
| Aviso | 12 × 16 (`space-3` × `space-4`) |
| Barra lateral | 16 × 8 (`space-4` × `space-2`) |
| Contenido, modal | 24 (`space-5`) |

3. **Gap según la relación entre elementos.** Cuanto más relacionados, más cerca.

| Relación | Gap | Ejemplos |
|---|---|---|
| Partes de un mismo elemento | 4 (`space-1`) | Etiqueta y descripción, ícono y texto de insignia, título y cuerpo de un aviso |
| Elemento y su complemento | 8 (`space-2`) | Ícono y texto de botón o navegación, etiqueta e insignia, botones vecinos |
| Elementos hermanos | 12 (`space-3`) | Avisos apilados, pista del slider y su valor, filas (relleno vertical) |
| Grupos distintos | 24 (`space-5`) | Entre secciones, texto y control de una fila, cuerpo y acciones de un modal |
| Bloques de pantalla | 32 (`space-6`) | Margen del modal con el borde de pantalla |

4. **Alturas fijas.** Insignia 20, segmento 28, control y botón 32, ítem de navegación 36, fila mínima 44, barra de acciones 56. Un componente nuevo usa una de estas alturas.
5. **Alineación.** Todo el texto de la barra lateral empieza a 20 px del borde (8 de la barra + 12 del ítem). Todos los controles de una ventana terminan en el mismo borde derecho, a 24 px del borde del contenido.

## Ventana completa

```
┌─────────── 200 ───────────┬──────────────── 680 ────────────────┐
│ ↕16                       │ ↕24                                 │
│ ↔8 ┌ Ajustes ──────────┐  │ ↔24 Gráficos                    ↔24 │
│    └ ↕16 ──────────────┘  │     ↕16                             │
│    ┌ 36 ▣ ↔8 Video ─────┐ │     Calidad                         │
│    └────────────────────┘ │     ↕4                              │
│      ↕2                   │     ┌ fila ↕12 ─────────── 240 ──┐  │
│    ┌ 36 ▣ Gráficos ─────┐ │     │ Etiqueta    ↔24   [control]│  │
│    └────────────────────┘ │     │ ↕4 Descripción             │  │
│                           │     └ ↕12 ───────────────────────┘  │
│                           │     ↕24                             │
│                           │     Estilo visual                   │
├───────────────────────────┼─────────────────────────────────────┤
│                           │ ↔24 Ayuda  Restablecer ↔8 [Aplicar] │ 56
└───────────────────────────┴─────────────────────────────────────┘
```

## Tokens por componente

### Ventana de ajustes

| Token | Valor | Dónde |
|---|---|---|
| `window-padding` | 0 | La ventana no tiene relleno propio; lo ponen la barra lateral y el contenido. |
| `window-radius` | 12px | Esquinas de la ventana. |
| `content-padding` | 24px | Relleno del área de contenido en los cuatro lados. |
| `page-title-gap` | 16px | Del título de página a la primera sección. |
| `footer-padding` | 0 24px | Relleno horizontal de la barra de acciones (alto fijo 56 px). |
| `footer-gap` | 8px | Entre botones de la barra de acciones. |

### Barra lateral

| Token | Valor | Dónde |
|---|---|---|
| `nav-padding` | 16px 8px | Relleno de la barra lateral: vertical / horizontal. |
| `nav-title-padding` | 0 12px 16px | Título «Ajustes»: alinea su texto con el de los ítems y deja 16 px debajo. |
| `nav-item-gap` | 2px | Entre ítems de navegación (excepción a la escala de 4). |
| `nav-item-padding` | 0 12px | Relleno horizontal de cada ítem (alto fijo 36 px). |
| `nav-icon-gap` | 8px | Entre ícono y texto del ítem. |
| `nav-item-radius` | 8px | Esquinas del ítem seleccionado o en hover. |
| `nav-separator-margin` | 8px 12px | Margen del separador antes de «Acerca de». |

### Título de sección

| Token | Valor | Dónde |
|---|---|---|
| `section-gap-before` | 24px | Sobre cada título de sección (0 en la primera). |
| `section-gap-after` | 4px | Del título de sección a su primera fila. |

### Fila de ajuste

| Token | Valor | Dónde |
|---|---|---|
| `row-padding` | 12px 0 | Relleno vertical de la fila (alto mínimo 44 px). |
| `row-gap` | 24px | Mínimo entre el bloque de texto y la columna del control. |
| `row-text-gap` | 4px | Entre etiqueta y descripción. |
| `row-label-badge-gap` | 8px | Entre la etiqueta y su insignia o ícono de ayuda. |
| `row-indent` | 24px | Sangría de una opción dependiente. |

### Interruptor

| Token | Valor | Dónde |
|---|---|---|
| `toggle-size` | 36px 20px | Ancho × alto de la pista. |
| `toggle-knob` | 14px | Diámetro de la perilla. |
| `toggle-inset` | 3px | Distancia de la perilla al borde de la pista. |

### Slider

| Token | Valor | Dónde |
|---|---|---|
| `slider-hit-height` | 32px | Área táctil vertical; igual al alto de un control. |
| `slider-track` | 4px | Grosor de la pista. |
| `slider-knob` | 14px | Diámetro de la perilla. |
| `slider-value-gap` | 12px | Entre la pista y la lectura del valor. |
| `slider-value-width` | 44px | Ancho mínimo de la lectura, alineada a la derecha. |

### Desplegable

| Token | Valor | Dónde |
|---|---|---|
| `combo-padding` | 0 12px | Relleno horizontal (alto fijo 32 px). |
| `combo-chevron-gap` | 8px | Mínimo entre el texto y el chevron. |
| `popup-offset` | 4px | Distancia entre el control y su lista desplegada. |
| `popup-padding` | 4px | Relleno interno de la lista. |
| `popup-item-padding` | 0 12px | Relleno de cada opción (alto 32 px). |

### Selector segmentado

| Token | Valor | Dónde |
|---|---|---|
| `seg-padding` | 2px | Relleno del contenedor alrededor de los segmentos. |
| `seg-item-padding` | 0 12px | Relleno de cada segmento (alto 28 px). |
| `seg-item-radius` | 4px | Esquinas del segmento activo, dentro del contenedor de radius-md. |

### Botón

| Token | Valor | Dónde |
|---|---|---|
| `button-padding` | 0 16px | Relleno horizontal (alto fijo 32 px). |
| `button-icon-gap` | 8px | Entre ícono y texto. |
| `button-group-gap` | 8px | Entre botones vecinos. |

### Insignia

| Token | Valor | Dónde |
|---|---|---|
| `badge-padding` | 0 8px | Relleno horizontal (alto fijo 20 px). |
| `badge-icon-gap` | 4px | Entre ícono de 12 px y texto. |

### Aviso

| Token | Valor | Dónde |
|---|---|---|
| `notice-padding` | 12px 16px | Relleno vertical / horizontal del aviso. |
| `notice-icon-gap` | 12px | Entre el ícono y el texto. |
| `notice-text-gap` | 4px | Entre título y cuerpo. |
| `notice-stack-gap` | 12px | Entre avisos apilados. |

### Tooltip

| Token | Valor | Dónde |
|---|---|---|
| `tooltip-padding` | 8px 12px | Relleno vertical / horizontal. |
| `tooltip-offset` | 8px | Distancia al elemento que lo abre. |
| `tooltip-text-gap` | 4px | Entre título y cuerpo. |
| `tooltip-max-width` | 280px | Ancho máximo antes de cortar líneas. |

### Modal de confirmación

| Token | Valor | Dónde |
|---|---|---|
| `modal-width` | 400px | Ancho fijo de la caja. |
| `modal-padding` | 24px | Relleno de la caja. |
| `modal-title-gap` | 8px | Del título al cuerpo. |
| `modal-actions-gap` | 24px | Del cuerpo a los botones. |
| `modal-screen-margin` | 32px | Margen mínimo con el borde de la pantalla. |

## ImGui

Los valores globales van en `ImGuiStyle` y los aplica `ImGuiModerno::ApplyTheme()` (ver Tema ImGui): `FramePadding` 12 × 6, `ItemSpacing` 8 × 8, `ItemInnerSpacing` 8 × 4, `CellPadding` 0 × 12, `IndentSpacing` 24. Las medidas de cada componente ya están dentro de sus widgets; si dibujas algo propio, usa las constantes de `ImGuiModerno::M` multiplicadas por `Dpi()`:

```cpp
namespace M {   // en imgui_moderno.h, a escala 1
    constexpr float space1 = 4, space2 = 8, space3 = 12, space4 = 16, space5 = 24, space6 = 32;
    constexpr float badgeH = 20, segH = 28, segPad = 2, controlH = 32, navItemH = 36, navItemGap = 2, rowMinH = 44, footerH = 56;
    constexpr float sidebarW = 200, controlW = 240, windowW = 880, windowH = 600;
    constexpr float toggleW = 36, toggleH = 20, toggleInset = 3;
    constexpr float sliderTrack = 4, sliderKnob = 14, sliderValueW = 44;
    constexpr float modalW = 400, tooltipMaxW = 280, iconSize = 16;
    constexpr float radiusSm = 4, radiusMd = 6, radiusLg = 8, radiusXl = 12, radiusFull = 999;
}
const float dpi = ImGuiModerno::Dpi();
ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(M::space4 * dpi, 0));   // button-padding
ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,  ImVec2(M::space2 * dpi, 0));   // button-group-gap
```

### Cómo ImGui afecta a los espacios

ImGui añade `ItemSpacing.y` (8 px) después de cada ítem. Para conseguir un hueco del sistema entre dos ítems, el hueco real es `gap = espacio extra + 8`. Los widgets ya lo compensan:

| Caso | Cómo se consigue |
|---|---|
| Etiqueta → descripción (4 px) | Se sube el cursor `8 − 4` px antes de la descripción. |
| Sobre un título de sección (24 px) | `Dummy(24 − 2 × 8)` antes del título. |
| Título de página → primera sección (16 px) | `Dummy(16 − 2 × 8)` = 0 tras el título. |
| Contenido → barra de acciones | El contenido reserva `footer-height + ItemSpacing.y` (56 + 8); si solo reservara 56, los 8 px de ImGui empujarían la barra fuera de la ventana y los botones quedarían pegados al borde inferior. |
| Barra de acciones | Relleno vertical `(56 − 32) / 2` = 12 px, horizontal 24 px; botones de 32 px centrados. |

Nunca muevas el cursor (`SetCursorPos`) después del último ítem de una ventana: ImGui lo marca como error. Si hace falta espacio al final, usa `ImGui::Dummy()`.
