# Guía del sistema ImGui Moderno

Sistema de diseño para llevar interfaces hechas con Dear ImGui (menús de ajustes, instaladores, herramientas de ports y emuladores) a un nivel limpio, moderno y comprensible para cualquier jugador, no solo para desarrolladores. Todo lo que define se puede implementar con ImGui: cada token tiene su equivalente en `ImGuiStyle` o en un color `ImGuiCol_*`.

## Principios

1. **Claridad antes que densidad.** ImGui tiende a amontonar opciones. Aquí cada ajuste tiene su fila, su explicación y aire alrededor.
2. **Un solo acento, monocromo.** `accent` (Grafito) marca solo lo que está activo o es la acción principal. Todo lo demás es gris neutro, y el color queda reservado para los estados.
3. **El fondo separa, no las líneas.** Ventanas y controles sin bordes; la jerarquía la dan los tonos de fondo (`bg-window` → `bg-sidebar` → `bg-frame`).
4. **Decir el efecto, no el parámetro.** «Bordes oscuros en personajes y objetos» en vez de «Outline pass enabled».
5. **Todo se usa con mouse, teclado y mando.** Nada depende solo del hover.

## Layout

La ventana de ajustes tiene siempre tres zonas (ver el componente **SettingsWindow**):

```
┌───────────────┬──────────────────────────────────────────┐
│ Ajustes       │ Gráficos                     (título)    │
│               │                                          │
│ ▣ Video       │ Calidad                      (sección)   │
│ ▣ Gráficos ◀  │ Perfil gráfico [Bajo|Medio|Alto|Ultra]   │
│ ▣ Cámara      │ ──────────────────────────────────────── │
│ ▣ Audio       │ Estilo visual                            │
│ ▣ Controles   │ Sombreado toon                  [ ●]     │
│ ▣ Accesib.    │ Descripción en gris                      │
│ ───────────── │ Contornos                       [ ●]     │
│ ▣ Acerca de   │                                          │
├───────────────┼──────────────────────────────────────────┤
│               │ Ayuda breve         Restablecer [Aplicar] │
└───────────────┴──────────────────────────────────────────┘
  200 px            contenido con scroll · relleno 24 px
```

- **Ventana**: 880 × 600 px a 1080p (`window-width`, `window-height`), centrada, `radius-xl`. Escala completa con el factor DPI.
- **Barra lateral**: 200 px fijos (`sidebar-width`), fondo `bg-sidebar`.
- **Contenido**: relleno `space-5` (24 px); es la única zona con scroll.
- **Barra de acciones**: 56 px (`footer-height`), separada por una línea `separator`. Los botones (32 px) quedan centrados: 12 px arriba y abajo, 24 px a los lados.
- **Filas de ajuste**: etiqueta y descripción a la izquierda; control a la derecha en una columna fija de 240 px (`control-width`). Todos los controles comparten el mismo borde derecho.

## Organización de la información

**Jerarquía de cuatro niveles**, nunca más:

1. Categoría (barra lateral): Video, Gráficos, Cámara, Audio, Controles, Accesibilidad, Acerca de.
2. Sección (título pequeño): Calidad, Estilo visual, Efectos, Avanzado.
3. Ajuste (fila): Contornos.
4. Ajuste dependiente (fila con sangría de 24 px): Grosor del contorno.

**Orden dentro de una categoría:**

1. Perfil o preset arriba (afecta todo lo de abajo).
2. Lo que más se cambia, después.
3. Lo exigente o técnico, más abajo, con su insignia «Exigente».
4. **Avanzado** siempre al final y colapsado (`CollapsibleSection`).

**Qué control usar:**

| Tipo de opción | Control |
|---|---|
| Sí / no | Toggle (switch) |
| 2 a 4 opciones | SegmentedControl |
| 5 o más opciones | Combo (desplegable) |
| Valor continuo | Slider con lectura del valor |
| Acción | Button |
| Consecuencia (reinicio, costo) | Badge en la fila |
| Resultado o error | Notice |
| Lectura de un valor | Value |
| Ayuda opcional | HelpMarker / Tooltip |

**Cuándo se aplican los cambios.** Por defecto, al instante (el usuario ve el efecto en el juego detrás del menú). Los que requieren reinicio llevan la insignia «Requiere reinicio» y se confirman con **Aplicar**.

## Tipografía

Una sola familia sans-serif para toda la interfaz: **Inter** (licencia OFL), con respaldo Segoe UI y system-ui. Para números, rutas y códigos de error, **JetBrains Mono**. No se usa ninguna fuente serif.

| Estilo | Tamaño / interlineado | Peso | Uso |
|---|---|---|---|
| `window-title` | 20 / 28 | 600 | Título de la ventana |
| `page-title` | 17 / 24 | 600 | Nombre de la categoría abierta |
| `section-label` | 12 / 16, +0.04em | 600 | Títulos de sección, en `text-muted` |
| `label` | 15 / 20 | 500 | Nombre del ajuste, botones, navegación |
| `body` | 15 / 20 | 400 | Texto de controles |
| `caption` | 13 / 18 | 400 | Descripción bajo un ajuste, en `text-secondary` |
| `value` | 13 / 18 mono | 400 | Lecturas: «1.10x», «60 Hz» |

Con Dear ImGui 1.92 o posterior las fuentes son dinámicas: se carga cada archivo una vez (Inter Regular, Medium y SemiBold, y JetBrains Mono) con `ImGuiModerno::LoadFonts()`, y el tamaño se elige al dibujar con `PushTextStyle(TextStyle_…)`. La escala DPI la aplica `ApplyTheme()` (`style.FontScaleDpi`). Si una fuente no está disponible, ese estilo usa la fuente por defecto con el tamaño correcto. Para íconos, fusiona la fuente de íconos sobre Inter Regular y Medium (ver Tema ImGui).

## Color y modos

Dos modos con los mismos nombres de token:

- **Oscuro** (por defecto): pensado para mostrarse sobre el juego. Grises neutros puros casi negros (`#141416` de fondo) y acento Grafito casi blanco `#ededef`.
- **Claro**: para el instalador o herramientas de escritorio. Grises casi blancos (`#fafafa`) y acento Grafito casi negro `#18181b`.

**Por qué Grafito.** El aspecto anticuado de ImGui viene de su azul saturado y de los grises teñidos de azul. Aquí los grises son neutros (sin tinte) y el acento es monocromo: lo activo se distingue por máximo contraste, no por color. Así el único color vivo de la interfaz son los estados (éxito, advertencia, error), que nunca se confunden con el acento.

Capas de fondo, de atrás hacia adelante: `bg-window` → `bg-sidebar` → `bg-frame` → `bg-frame-hover` → `bg-frame-active`. Popups y modales en `bg-popup`, con borde `border-subtle`.

Texto: `text-primary` para lo que se lee, `text-secondary` para descripciones, `text-muted` para títulos de sección y ayudas, `text-disabled` solo para lo no disponible.

Estados semánticos con pareja de fondo suave: `success` / `success-soft`, `warning` / `warning-soft`, `danger` / `danger-soft`. El rojo se usa solo para errores y acciones destructivas.

Todo el texto cumple al menos 4.5:1 de contraste sobre sus fondos en ambos modos. Los estados nunca se comunican solo con color: siempre van con ícono y texto.

## Íconos

- Set: estilo **Lucide** (licencia ISC), de línea, trazo de 1.75 px. El sistema trae dibujados con `ImDrawList` los que necesita una ventana de ajustes (`ImGuiModerno::Icon_*`: categorías, ayuda, abrir, restablecer, borrar), así que no hace falta cargar ninguna fuente. Para otros íconos puedes fusionar la fuente Lucide (IconFontCppHeaders, `ICON_LC_*`) y pasar el glifo a `SidebarItem`.
- Tamaño: 16 px (`icon-size`) en navegación, botones y avisos; 12 px en insignias.
- Color: hereda el del texto; en la categoría seleccionada, `accent`. Los íconos de estado usan `success`, `warning` o `danger`.
- Separación con el texto: `space-2` (8 px).
- Solo de línea; nunca rellenos ni a color.

| Uso | Ícono Lucide |
|---|---|
| Video | `monitor` |
| Gráficos | `palette` |
| Cámara | `camera` |
| Audio | `volume-2` |
| Controles | `gamepad-2` |
| Accesibilidad | `accessibility` |
| Acerca de | `info` |
| Ayuda | `circle-help` |
| Error | `circle-alert` |
| Correcto | `circle-check` |
| Requiere reinicio | `refresh-cw` |
| Exigente | `zap` |
| Abrir archivo | `folder-open` |
| Borrar | `trash-2` |
| Desplegar | `chevron-down` |
| Seleccionado | `check` |

## Forma y espacio

- Toda medida sale de la escala de 4: 4, 8, 12, 16, 24, 32 (`space-1` a `space-6`). Las únicas excepciones son 1 px (líneas), 2 px (separación entre ítems de navegación y relleno del selector segmentado) y 3 px (margen interno de la perilla del switch).
- Cada componente tiene sus propios tokens de padding, gap y tamaño en la familia **Medidas** (por ejemplo `button-padding`, `row-gap`). La sección **Medidas y espaciado** reúne todos en una sola tabla, y cada componente repite los suyos.
- Radios: 4 px insignias, 6 px controles, 8 px popups y navegación, 12 px ventanas, completo para switch y perillas.
- Sin sombras: ImGui no las dibuja de forma nativa, y el sistema no las necesita.
- Alturas fijas: 20 px insignias, 28 px segmentos, 32 px controles y botones, 36 px ítems de navegación, 44 px mínimo por fila, 56 px barra de acciones.

## Movimiento

- Una sola duración para todo: **120 ms** (`M::motion`), con salida suave. Rápido para no hacer esperar a nadie y suficiente para que el ojo siga el cambio.
- Solo se anima lo que cambia de sitio o de estado: la perilla del interruptor, la pastilla del selector segmentado y el fondo de la categoría seleccionada.
- Nada se anima en bucle ni para llamar la atención. Sin rebotes ni efectos elásticos.

## Escritura

La interfaz se escribe en **español, inglés y portugués (Brasil)**, con las mismas reglas en los tres (ver la tabla por idioma en `AGENTS.md`). Cada idioma se escribe para quien lo lee, no se traduce palabra por palabra: «Restablecer» es «Reset» y «Redefinir», no «Re-establish».

- Español neutro, tuteo («Elige», «Puedes»), sin regionalismos. Inglés en segunda persona y *sentence case*. Portugués de Brasil con «você».
- Etiquetas: sustantivo corto, mayúscula inicial y sin dos puntos («Campo de visión»).
- Botones: verbo en infinitivo («Aplicar», «Elegir ROM»). Nunca «OK».
- Descripciones: una línea, dicen qué cambia en pantalla.
- Errores: qué pasó (título) + qué hacer (cuerpo) + código en su propia línea en mono («Código CTL-0042»). Nunca un volcado técnico solo.
- Valores siempre con unidad: «90 Hz», «30 %», «1.10x».
- El idioma se cambia al momento desde Accesibilidad → Idioma, con el nombre de cada idioma escrito en ese idioma («Español», «English», «Português»).
- Las etiquetas cortas (segmentos, botones) deben caber en los tres idiomas: el portugués es el más largo.
- Sin signos de exclamación en mensajes del sistema.

## Accesibilidad y mando

- Navegación completa con teclado y mando (`ImGuiConfigFlags_NavEnableGamepad`); el foco se dibuja con `accent`.
- Opción de escala de interfaz (100 %, 125 %, 150 %, 200 %) en Accesibilidad: `ApplyTheme(dark, dpiBase × escala)` multiplica todos los tokens de tamaño y las fuentes.
- Los sliders se mueven con izquierda/derecha del teclado o la cruceta cuando tienen el foco; el ícono de ayuda es enfocable para que su tooltip también aparezca con mando.
- Nunca deshabilitar sin explicar el motivo en la descripción.

## Cómo aplicarlo

- `imgui_moderno.h` implementa todo el sistema: `ApplyTheme()` aplica los tokens a `ImGuiStyle` en ambos modos y cada componente tiene su widget. La referencia está en **docs/api.md**.
- **AGENTS.md** explica paso a paso cómo aplicarlo o migrar una interfaz existente (pensado también para asistentes de IA), con la lista de verificación final.
- **docs/tema-imgui.md** detalla el mapeo de tokens a `ImGuiStyle` y la carga de fuentes.
- Cada ficha de `docs/componentes/` incluye reglas de uso, medidas y el código para usarlo.
- `imgui_moderno_demo.cpp` es una ventana completa de ejemplo.
