# SectionHeader

Título pequeño que agrupa filas relacionadas dentro de una categoría («Estilo visual», «Cámara», «Avanzado»).

- Estilo `section-label`: 12 px, peso 600, en `text-muted`.
- `space-5` arriba (salvo la primera sección) y `space-1` abajo.
- Máximo 5 o 6 filas por sección. Si hay más, divide en dos secciones.
- La sección **Avanzado** va siempre al final y puede ir colapsada (`ImGui::CollapsingHeader`).
- Sin línea propia: la separación la dan las filas.

## Código

```cpp
ImGuiModerno::SectionHeader("Perfil", /*first=*/true);      // la primera de la página, sin hueco arriba
if (ImGuiModerno::BeginSettingRows("##perfil")) { /* filas */ ImGuiModerno::EndSettingRows(); }

ImGuiModerno::SectionHeader("Estilo visual");
if (ImGuiModerno::BeginSettingRows("##estilo")) { /* filas */ ImGuiModerno::EndSettingRows(); }

// Avanzado: última sección, colapsada por defecto
if (ImGuiModerno::CollapsibleSection("Avanzado") && ImGuiModerno::BeginSettingRows("##avanzado")) {
    /* filas */
    ImGuiModerno::EndSettingRows();
}
```

`SectionHeader` debe ir seguido de filas: no lo dejes como último elemento de la ventana.

Firmas completas en `docs/api.md`.

## Medidas

| Token | Valor | Escala | Dónde |
|---|---|---|---|
| `section-gap-before` | 24px | `space-5` | Sobre cada título de sección (0 en la primera). |
| `section-gap-after` | 4px | `space-1` | Del título de sección a su primera fila. |
