# Notice

Aviso en bloque para resultados que el usuario debe leer: un error de instalación, un cambio pendiente de reinicio o una verificación correcta.

## Código

```cpp
ImGuiModerno::Notice(ImGuiModerno::Tone_Danger, "No se pudo preparar el juego",
    "La ROM no es la versión EUR. Elige un archivo .3ds o .cci de la versión EUR.", "Código ROM-0013");
ImGuiModerno::Notice(ImGuiModerno::Tone_Warning, "El cambio se aplicará al reiniciar el juego",
    "El sonido seguirá usando el dispositivo anterior hasta entonces.");
ImGuiModerno::Notice(ImGuiModerno::Tone_Success, "Tu instalación está completa",
    "Los archivos del sistema coinciden con la versión publicada.");
```

Ocupa todo el ancho disponible y calcula su alto según el texto. Para separarlo de lo que sigue, añade `ImGui::Dummy(ImVec2(0, M::space2 * Dpi()))`.

Firmas completas en `docs/api.md`.

## Medidas

| Token | Valor | Escala | Dónde |
|---|---|---|---|
| `notice-padding` | 12px 16px | `space-3 / space-4` | Relleno vertical / horizontal del aviso. |
| `notice-icon-gap` | 12px | `space-3` | Entre el ícono y el texto. |
| `notice-text-gap` | 4px | `space-1` | Entre título y cuerpo. |
| `notice-stack-gap` | 12px | `space-3` | Entre avisos apilados. |

## Cómo se escribe un aviso

1. **Título**: qué pasó, en una frase. «No se pudo preparar el juego».
2. **Cuerpo**: por qué y qué hacer, en una o dos frases. «La ROM no es la versión EUR. Elige un archivo .3ds o .cci de la versión EUR.»
3. **Código** (solo errores): en `value` (mono), para buscarlo o reportarlo. Nunca mostrar solo el código o un volcado técnico.

## Variantes

| Variante | Fondo | Ícono | Cuándo |
|---|---|---|---|
| Error | `danger-soft` | `circle-alert` en `danger` | Algo falló y bloquea |
| Advertencia | `warning-soft` | `refresh-cw` o `triangle-alert` en `warning` | Hay que hacer algo después |
| Correcto | `success-soft` | `circle-check` en `success` | Confirmar que terminó bien |

El texto siempre en `text-primary`; el color lo llevan el fondo y el ícono. Relleno `space-3` × `space-4`, `radius-lg`.
