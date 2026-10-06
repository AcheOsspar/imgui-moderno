# SidebarNav

Lista vertical de categorías con ícono y texto; la seleccionada se marca con fondo `accent-soft` e ícono en `accent`.

## Código

```cpp
static const char* const categorias[] = { "Video", "Gráficos", "Cámara", "Audio", "Controles", "Accesibilidad", "Acerca de" };
static const ImGuiModerno::Icon iconos[] = { ImGuiModerno::Icon_Monitor, ImGuiModerno::Icon_Palette, ImGuiModerno::Icon_Camera,
    ImGuiModerno::Icon_Volume, ImGuiModerno::Icon_Gamepad, ImGuiModerno::Icon_Accessibility, ImGuiModerno::Icon_Info };
ImGuiModerno::BeginSidebar("Ajustes");
for (int i = 0; i < IM_ARRAYSIZE(categorias); i++) {
    if (i == 6) ImGuiModerno::SidebarSeparator();                     // antes de «Acerca de»
    if (ImGuiModerno::SidebarItem(categorias[i], cat == i, iconos[i])) cat = i;
}
ImGuiModerno::EndSidebar();
```

Los íconos (`Icon_*`) los dibuja el propio sistema; no hace falta cargar ninguna fuente. También puedes pasar un glifo de una fuente de íconos fusionada, o nada.

Al cambiar de categoría, el fondo del ítem seleccionado se desliza hasta el nuevo en 120 ms (`M::motion`).

Firmas completas en `docs/api.md`.

## Medidas

| Token | Valor | Escala | Dónde |
|---|---|---|---|
| `nav-padding` | 16px 8px | `space-4 / space-2` | Relleno de la barra lateral: vertical / horizontal. |
| `nav-title-padding` | 0 12px 16px | `space-3 / space-4` | Título «Ajustes»: alinea su texto con el de los ítems y deja 16 px debajo. |
| `nav-item-gap` | 2px | — | Entre ítems de navegación (excepción a la escala de 4). |
| `nav-item-padding` | 0 12px | `space-3` | Relleno horizontal de cada ítem (alto fijo 36 px). |
| `nav-icon-gap` | 8px | `space-2` | Entre ícono y texto del ítem. |
| `nav-item-radius` | 8px | `radius-lg` | Esquinas del ítem seleccionado o en hover. |
| `nav-separator-margin` | 8px 12px | `space-2 / space-3` | Margen del separador antes de «Acerca de». |

## Orden de categorías

Siempre en este orden, de lo más consultado a lo menos:

1. **Video**: pantalla completa, resolución, V-Sync, límite de FPS
2. **Gráficos**: perfil, estilo visual, efectos, texturas
3. **Cámara**: campo de visión, interpolación, cámara libre
4. **Audio**: volúmenes
5. **Controles**: asignación de botones, mando, giroscopio
6. **Accesibilidad**: tamaño de interfaz, subtítulos, contraste
7. Separador
8. **Acerca de**: versión, licencias, créditos

## Estados

| Estado | Fondo | Texto | Ícono |
|---|---|---|---|
| Reposo | ninguno | `text-secondary` | `text-secondary` |
| Hover | `bg-frame-hover` | `text-primary` | `text-primary` |
| Seleccionado | `accent-soft` | `text-primary` | `accent` |

Cada ítem mide `nav-item-height` (36 px), radio `radius-lg`, ícono de `icon-size` (16 px) a `space-2` del texto. Las flechas arriba y abajo del mando recorren las categorías.
