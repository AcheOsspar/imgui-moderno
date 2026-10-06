// imgui_moderno_demo.cpp — Demo de ImGui Moderno: ventana de ajustes de un emulador.
// Copyright (c) 2026 AcheOsspar. Licencia MIT (ver misc/moderno/LICENSE).
// Muestra todos los componentes del sistema. Añade este archivo a tu proyecto y llama a
// ImGuiModerno::ShowDemoWindow(&open) dentro de tu frame, después de ImGuiModerno::ApplyTheme().

#include "imgui_moderno.h"

namespace ImGuiModerno {

namespace {

struct DemoConfig {
    // Video
    bool  fullscreen = false;
    int   resolution = 2;
    bool  vsync = true;
    int   fpsLimit = 1;
    // Gráficos
    int   profile = 2;
    bool  outlines = true;
    bool  interpolation = true;    // valores del perfil Alto
    int   interpolationFrames = 0;
    int   textureFilter = 2;
    bool  asyncShaders = true;
    // Cámara
    float fov = 1.0f;
    bool  freeCamera = false;
    // Audio
    float volMaster = 80.0f, volMusic = 60.0f, volEffects = 70.0f;
    int   audioBackend = 0;
    // Controles
    bool  rumble = true;
    bool  gyro = false;
    // Accesibilidad
    int   theme = 0;   // 0 oscuro, 1 claro
    int   uiScale = 0;
    bool  subtitles = true;
};

// Perfiles gráficos: cada uno fija varias opciones. Tocar una a mano deja el perfil en «Personalizado» (-1).
void ApplyProfile(DemoConfig& c, int profile) {
    static const int  filters[4] = { 0, 1, 2, 3 };
    static const bool outlines[4] = { false, true, true, true };
    static const bool interp[4]   = { false, false, true, true };
    c.textureFilter = filters[profile];
    c.outlines = outlines[profile];
    c.interpolation = interp[profile];
}

bool SameConfig(const DemoConfig& a, const DemoConfig& b) {
    return a.fullscreen == b.fullscreen && a.resolution == b.resolution && a.vsync == b.vsync && a.fpsLimit == b.fpsLimit &&
           a.profile == b.profile && a.outlines == b.outlines && a.interpolation == b.interpolation &&
           a.interpolationFrames == b.interpolationFrames && a.textureFilter == b.textureFilter && a.asyncShaders == b.asyncShaders &&
           a.fov == b.fov && a.freeCamera == b.freeCamera && a.volMaster == b.volMaster && a.volMusic == b.volMusic &&
           a.volEffects == b.volEffects && a.audioBackend == b.audioBackend && a.rumble == b.rumble && a.gyro == b.gyro &&
           a.subtitles == b.subtitles;   // tema y tamaño de interfaz se aplican al momento: no cuentan como cambios pendientes
}

enum Category { Cat_Video, Cat_Graphics, Cat_Camera, Cat_Audio, Cat_Controls, Cat_Accessibility, Cat_About, Cat_COUNT };
const char* const kCategoryNames[Cat_COUNT] = { "Video", "Gráficos", "Cámara", "Audio", "Controles", "Accesibilidad", "Acerca de" };

void PageVideo(DemoConfig& c) {
    static const char* const resolutions[] = { "Nativa · 400×240", "x2 · 800×480", "x3 · 1200×720", "x4 · 1600×960", "x5 · 2000×1200", "x6 · 2400×1440" };
    static const char* const fps[] = { "30", "60", "90", "Sin límite" };

    SectionHeader("Pantalla", true);
    if (BeginSettingRows("##pantalla")) {
        SettingRow("Pantalla completa", "Ocupa todo el monitor. También con Alt+Intro.");
        ToggleSwitch("##fullscreen", &c.fullscreen);
        RowOptions res; res.badge = "Exigente"; res.badgeTone = Tone_Warning;
        SettingRow("Resolución interna", "Dibuja el juego a más resolución que la consola original.", c.resolution >= 4 ? res : RowOptions());
        Combo("##resolucion", &c.resolution, resolutions, IM_ARRAYSIZE(resolutions));
        SettingRow("Sincronización vertical", "Evita cortes en la imagen. Puede añadir un poco de retraso.");
        ToggleSwitch("##vsync", &c.vsync);
        SettingRow("Límite de FPS", "Cuadros por segundo como máximo.");
        Segmented("##fps", &c.fpsLimit, fps, IM_ARRAYSIZE(fps));
        EndSettingRows();
    }
}

void PageGraphics(DemoConfig& c) {
    static const char* const profiles[] = { "Bajo", "Medio", "Alto", "Ultra" };
    static const char* const filters[] = { "Ninguno", "Bilineal", "Bicúbico", "xBRZ · 2x", "xBRZ · 4x", "ScaleForce" };
    static const char* const frames[] = { "1 cuadro", "2 cuadros" };

    SectionHeader("Perfil", true);
    if (BeginSettingRows("##perfil")) {
        RowOptions custom; custom.badge = "Personalizado"; custom.badgeTone = Tone_Neutral;
        SettingRow("Perfil gráfico", "Ajusta varias opciones a la vez según la potencia de tu equipo.", c.profile < 0 ? custom : RowOptions());
        if (Segmented("##perfil", &c.profile, profiles, IM_ARRAYSIZE(profiles))) ApplyProfile(c, c.profile);
        EndSettingRows();
    }

    SectionHeader("Estilo visual");
    if (BeginSettingRows("##estilo")) {
        SettingRow("Contornos", "Bordes oscuros en personajes y objetos.");
        if (ToggleSwitch("##contornos", &c.outlines)) c.profile = -1;

        RowOptions interp;
        interp.badge = "Exigente";
        interp.helpTitle = "Interpolación visual";
        interp.helpBody = "Dibuja cuadros intermedios para que el movimiento se vea más fluido. No cambia la velocidad del juego.";
        SettingRow("Interpolación visual", "Movimiento más suave en juegos a 30 FPS.", interp);
        if (ToggleSwitch("##interp", &c.interpolation)) c.profile = -1;

        RowOptions dep; dep.dependent = true;
        ImGui::BeginDisabled(!c.interpolation);
        SettingRow("Cuadros intermedios", c.interpolation ? "Más cuadros, más fluidez y más consumo." : "Activa Interpolación visual para elegir cuántos.", dep);
        Segmented("##frames", &c.interpolationFrames, frames, IM_ARRAYSIZE(frames));
        ImGui::EndDisabled();

        SettingRow("Filtro de texturas", "Suaviza o redibuja las texturas al ampliarlas.");
        if (Combo("##filtro", &c.textureFilter, filters, IM_ARRAYSIZE(filters))) c.profile = -1;
        EndSettingRows();
    }

    if (CollapsibleSection("Avanzado") && BeginSettingRows("##avanzado")) {
        RowOptions restart; restart.badge = "Requiere reinicio";
        SettingRow("Shaders asíncronos", "Reduce tirones la primera vez que aparece un efecto.", restart);
        ToggleSwitch("##async", &c.asyncShaders);

        bool rayTracing = false;
        ImGui::BeginDisabled();
        SettingRow("Trazado de rayos", "No disponible: necesita una GPU compatible con Vulkan 1.3.");
        ToggleSwitch("##rt", &rayTracing);
        ImGui::EndDisabled();
        EndSettingRows();
    }
}

void PageCamera(DemoConfig& c) {
    SectionHeader("Vista", true);
    if (BeginSettingRows("##vista")) {
        SettingRow("Campo de visión", "Amplía lo que ves a los lados. Clic derecho para volver a 1.00x.");
        SliderFloat("##fov", &c.fov, 0.8f, 1.5f, "%.2fx", 1.0f, 0.01f);
        SettingRow("Cámara libre", "Mueve la cámara con el stick derecho. Algunos juegos no lo admiten.");
        ToggleSwitch("##freecam", &c.freeCamera);
        EndSettingRows();
    }
}

void PageAudio(DemoConfig& c) {
    static const char* const backends[] = { "Automático", "WASAPI", "XAudio2" };
    if (c.audioBackend != 0) {
        Notice(Tone_Warning, "El cambio se aplicará al reiniciar el juego", "El sonido seguirá usando el dispositivo anterior hasta entonces.");
        ImGui::Dummy(ImVec2(0, M::space2 * Dpi()));
    }
    SectionHeader("Volumen", true);
    if (BeginSettingRows("##volumen")) {
        SettingRow("Volumen general");
        SliderFloat("##master", &c.volMaster, 0, 100, "%.0f %%", 80.0f, 1.0f);
        SettingRow("Música");
        SliderFloat("##music", &c.volMusic, 0, 100, "%.0f %%", 60.0f, 1.0f);
        SettingRow("Efectos");
        SliderFloat("##fx", &c.volEffects, 0, 100, "%.0f %%", 70.0f, 1.0f);
        EndSettingRows();
    }
    SectionHeader("Salida");
    if (BeginSettingRows("##salida")) {
        RowOptions restart; restart.badge = "Requiere reinicio";
        SettingRow("Motor de audio", "Automático elige el mejor para tu equipo.", restart);
        Segmented("##backend", &c.audioBackend, backends, IM_ARRAYSIZE(backends));
        EndSettingRows();
    }
}

void PageControls(DemoConfig& c) {
    Notice(Tone_Danger, "No se detecta ningún mando",
           "Conecta un mando por USB o Bluetooth y pulsa cualquier botón. Si ya está conectado, prueba otro puerto.",
           "Código CTL-0042");
    ImGui::Dummy(ImVec2(0, M::space2 * Dpi()));
    SectionHeader("Mando", true);
    if (BeginSettingRows("##mando")) {
        SettingRow("Vibración", "El mando vibra con los golpes y las explosiones.");
        ToggleSwitch("##rumble", &c.rumble);
        RowOptions beta; beta.badge = "Beta"; beta.badgeTone = Tone_Neutral;
        SettingRow("Giroscopio", "Apunta inclinando el mando. Solo mandos con sensor de movimiento.", beta);
        ToggleSwitch("##gyro", &c.gyro);
        EndSettingRows();
    }
}

void PageAccessibility(DemoConfig& c) {
    static const char* const themes[] = { "Oscuro", "Claro" };
    static const char* const scales[] = { "100 %", "125 %", "150 %", "200 %" };
    SectionHeader("Interfaz", true);
    if (BeginSettingRows("##interfaz")) {
        SettingRow("Tema", "Se aplica al momento.");
        Segmented("##tema", &c.theme, themes, IM_ARRAYSIZE(themes));
        SettingRow("Tamaño de interfaz", "Agranda textos y controles de todos los menús.");
        Segmented("##escala", &c.uiScale, scales, IM_ARRAYSIZE(scales));
        SettingRow("Subtítulos", "Muestra los diálogos como texto en pantalla.");
        ToggleSwitch("##subs", &c.subtitles);
        EndSettingRows();
    }
}

void PageAbout(bool* requestReset) {
    SectionHeader("Versión", true);
    if (BeginSettingRows("##version")) {
        SettingRow("Emulador");
        Value("v2.4.1 · build 8f3c21a");
        SettingRow("Dear ImGui");
        Value(IMGUI_VERSION);
        SettingRow("Archivos del sistema");
        Badge("Verificada", Tone_Success);
        EndSettingRows();
    }
    ImGui::Dummy(ImVec2(0, M::space2 * Dpi()));
    Notice(Tone_Success, "Tu instalación está completa", "Los archivos del sistema coinciden con la versión publicada.");

    SectionHeader("Zona de peligro");
    if (BeginSettingRows("##peligro")) {
        SettingRow("Restablecer todo", "Vuelve a la configuración de fábrica. Tus partidas guardadas no se borran.");
        if (DangerButton("Restablecer todo")) *requestReset = true;
        EndSettingRows();
    }
}

} // namespace

void ShowDemoWindow(bool* open) {
    static DemoConfig cfg, applied;
    static int category = Cat_Graphics;
    static float baseDpi = 0.0f;
    static int lastTheme = -1, lastScale = -1;
    static const float scales[] = { 1.0f, 1.25f, 1.5f, 2.0f };

    // Tema y escala se aplican al momento, antes de abrir ninguna ventana (fuera de cualquier Push/Pop).
    if (baseDpi == 0.0f) { baseDpi = Dpi(); lastTheme = IsDark() ? 0 : 1; cfg.theme = applied.theme = lastTheme; lastScale = cfg.uiScale; }
    if (cfg.theme != lastTheme || cfg.uiScale != lastScale) {
        ApplyTheme(cfg.theme == 0, baseDpi * scales[cfg.uiScale]);
        lastTheme = cfg.theme; lastScale = cfg.uiScale;
    }

    bool requestReset = false;
    if (BeginSettingsWindow("Ajustes##ImGuiModernoDemo", open)) {
        BeginSidebar("Ajustes");
        for (int i = 0; i < Cat_COUNT; i++) {
            if (i == Cat_About) SidebarSeparator();
            if (SidebarItem(kCategoryNames[i], category == i)) category = i;
        }
        EndSidebar();

        if (BeginSettingsContent(kCategoryNames[category])) {
            switch (category) {
            case Cat_Video:         PageVideo(cfg); break;
            case Cat_Graphics:      PageGraphics(cfg); break;
            case Cat_Camera:        PageCamera(cfg); break;
            case Cat_Audio:         PageAudio(cfg); break;
            case Cat_Controls:      PageControls(cfg); break;
            case Cat_Accessibility: PageAccessibility(cfg); break;
            case Cat_About:         PageAbout(&requestReset); break;
            }
        }
        EndSettingsContent();

        // Barra de acciones: Aplicar solo aparece con cambios pendientes.
        const bool dirty = !SameConfig(cfg, applied);
        BeginSettingsFooter();
        GhostButton("Ayuda");
        ItemTooltip(nullptr, "Abre la guía del emulador en el navegador.");
        const float gap = M::space2 * Dpi();
        const float right = ButtonWidth("Restablecer") + (dirty ? gap + ButtonWidth("Aplicar") : 0.0f);
        SameLineRight(right);
        if (GhostButton("Restablecer")) { const int t = cfg.theme, s = cfg.uiScale; cfg = DemoConfig(); cfg.theme = t; cfg.uiScale = s; }
        if (dirty) {
            ImGui::SameLine(0, gap);
            if (PrimaryButton("Aplicar")) applied = cfg;
        }
        EndSettingsFooter();

        // Esc cierra; si hay cambios sin aplicar, pregunta antes.
        if (open && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId) &&
            ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
            if (dirty) ImGui::OpenPopup("##cerrar");
            else *open = false;
        }
        if (requestReset) ImGui::OpenPopup("##restablecer");

        if (ConfirmModal("##cerrar", "¿Cerrar sin aplicar los cambios?", "Los ajustes volverán a como estaban al abrir esta ventana.",
                         "Cerrar sin aplicar", true, "Seguir editando") == ConfirmResult_Confirm) {
            const int t = applied.theme, s = applied.uiScale;
            cfg = applied; cfg.theme = t; cfg.uiScale = s;
            if (open) *open = false;
        }
        if (ConfirmModal("##restablecer", "¿Restablecer todos los ajustes?", "Se perderá tu configuración. Tus partidas guardadas no se verán afectadas.",
                         "Restablecer todo") == ConfirmResult_Confirm) {
            cfg = applied = DemoConfig();
        }
    }
    EndSettingsWindow();
}

} // namespace ImGuiModerno
