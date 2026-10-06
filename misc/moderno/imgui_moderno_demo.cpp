// imgui_moderno_demo.cpp — Demo de ImGui Moderno: ventana de ajustes de un emulador.
// Copyright (c) 2026 AcheOsspar. Licencia MIT (ver misc/moderno/LICENSE).
// Muestra todos los componentes del sistema, en español, inglés y portugués (Accesibilidad → Idioma).
// Añade este archivo a tu proyecto y llama a ImGuiModerno::ShowDemoWindow(&open) dentro de tu frame,
// después de ImGuiModerno::ApplyTheme().

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
    // Accesibilidad (tema, escala e idioma se aplican al momento)
    int   theme = 0;      // 0 oscuro, 1 claro
    int   uiScale = 0;
    int   language = 0;   // Language_*
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
           a.subtitles == b.subtitles;   // tema, tamaño de interfaz e idioma se aplican al momento: no cuentan como pendientes
}

// Conserva lo que se aplica al momento al volver a otros valores.
void KeepInstant(DemoConfig& dst, const DemoConfig& src) {
    dst.theme = src.theme; dst.uiScale = src.uiScale; dst.language = src.language;
}

enum Category { Cat_Video, Cat_Graphics, Cat_Camera, Cat_Audio, Cat_Controls, Cat_Accessibility, Cat_About, Cat_COUNT };
const Icon kCategoryIcons[Cat_COUNT] = { Icon_Monitor, Icon_Palette, Icon_Camera, Icon_Volume, Icon_Gamepad, Icon_Accessibility, Icon_Info };

const char* CategoryName(int c) {
    switch (c) {
    case Cat_Video:         return Tr("Video", "Video", "Vídeo");
    case Cat_Graphics:      return Tr("Gráficos", "Graphics", "Gráficos");
    case Cat_Camera:        return Tr("Cámara", "Camera", "Câmera");
    case Cat_Audio:         return Tr("Audio", "Audio", "Áudio");
    case Cat_Controls:      return Tr("Controles", "Controls", "Controles");
    case Cat_Accessibility: return Tr("Accesibilidad", "Accessibility", "Acessibilidade");
    default:                return Tr("Acerca de", "About", "Sobre");
    }
}

RowOptions Badge_(const char* text, Tone tone) { RowOptions o; o.badge = text; o.badgeTone = tone; return o; }
const char* RestartText()  { return Tr("Requiere reinicio", "Restart required", "Requer reinício"); }
const char* DemandingText() { return Tr("Exigente", "Demanding", "Exigente"); }

void PageVideo(DemoConfig& c) {
    const char* const resolutions[] = { Tr("Nativa · 400×240", "Native · 400×240", "Nativa · 400×240"),
        "x2 · 800×480", "x3 · 1200×720", "x4 · 1600×960", "x5 · 2000×1200", "x6 · 2400×1440" };
    const char* const fps[] = { "30", "60", "90", Tr("Sin límite", "Unlimited", "Sem limite") };

    SectionHeader(Tr("Pantalla", "Display", "Tela"), true);
    if (BeginSettingRows("##pantalla")) {
        SettingRow(Tr("Pantalla completa", "Full screen", "Tela cheia"),
                   Tr("Ocupa todo el monitor. También con Alt+Intro.", "Fills the whole monitor. Also with Alt+Enter.", "Ocupa o monitor inteiro. Também com Alt+Enter."));
        ToggleSwitch("##fullscreen", &c.fullscreen);
        SettingRow(Tr("Resolución interna", "Internal resolution", "Resolução interna"),
                   Tr("Dibuja el juego a más resolución que la consola original.", "Renders the game at a higher resolution than the original console.", "Renderiza o jogo em resolução maior que a do console original."),
                   c.resolution >= 4 ? Badge_(DemandingText(), Tone_Warning) : RowOptions());
        Combo("##resolucion", &c.resolution, resolutions, IM_ARRAYSIZE(resolutions));
        SettingRow(Tr("Sincronización vertical", "Vertical sync", "Sincronização vertical"),
                   Tr("Evita cortes en la imagen. Puede añadir un poco de retraso.", "Prevents screen tearing. May add a little input lag.", "Evita cortes na imagem. Pode adicionar um pouco de atraso."));
        ToggleSwitch("##vsync", &c.vsync);
        SettingRow(Tr("Límite de FPS", "FPS limit", "Limite de FPS"),
                   Tr("Cuadros por segundo como máximo.", "Maximum frames per second.", "Quadros por segundo no máximo."));
        Segmented("##fps", &c.fpsLimit, fps, IM_ARRAYSIZE(fps));
        EndSettingRows();
    }
}

void PageGraphics(DemoConfig& c) {
    const char* const profiles[] = { Tr("Bajo", "Low", "Baixo"), Tr("Medio", "Medium", "Médio"), Tr("Alto", "High", "Alto"), "Ultra" };
    const char* const filters[] = { Tr("Ninguno", "None", "Nenhum"), Tr("Bilineal", "Bilinear", "Bilinear"), Tr("Bicúbico", "Bicubic", "Bicúbico"),
                                    "xBRZ · 2x", "xBRZ · 4x", "ScaleForce" };
    const char* const frames[] = { Tr("1 cuadro", "1 frame", "1 quadro"), Tr("2 cuadros", "2 frames", "2 quadros") };
    const char* interpName = Tr("Interpolación visual", "Motion interpolation", "Interpolação visual");

    SectionHeader(Tr("Perfil", "Profile", "Perfil"), true);
    if (BeginSettingRows("##perfil")) {
        SettingRow(Tr("Perfil gráfico", "Graphics profile", "Perfil gráfico"),
                   Tr("Ajusta varias opciones a la vez según la potencia de tu equipo.", "Sets several options at once to match your computer.", "Ajusta várias opções de uma vez conforme a potência do seu computador."),
                   c.profile < 0 ? Badge_(Tr("Personalizado", "Custom", "Personalizado"), Tone_Neutral) : RowOptions());
        if (Segmented("##perfil", &c.profile, profiles, IM_ARRAYSIZE(profiles))) ApplyProfile(c, c.profile);
        EndSettingRows();
    }

    SectionHeader(Tr("Estilo visual", "Visual style", "Estilo visual"));
    if (BeginSettingRows("##estilo")) {
        SettingRow(Tr("Contornos", "Outlines", "Contornos"),
                   Tr("Bordes oscuros en personajes y objetos.", "Dark edges on characters and objects.", "Bordas escuras em personagens e objetos."));
        if (ToggleSwitch("##contornos", &c.outlines)) c.profile = -1;

        RowOptions interp = Badge_(DemandingText(), Tone_Warning);
        interp.helpTitle = interpName;
        interp.helpBody = Tr("Dibuja cuadros intermedios para que el movimiento se vea más fluido. No cambia la velocidad del juego.",
                             "Draws in-between frames so motion looks smoother. It does not change the game speed.",
                             "Desenha quadros intermediários para o movimento parecer mais fluido. Não muda a velocidade do jogo.");
        SettingRow(interpName, Tr("Movimiento más suave en juegos a 30 FPS.", "Smoother motion in 30 FPS games.", "Movimento mais suave em jogos a 30 FPS."), interp);
        if (ToggleSwitch("##interp", &c.interpolation)) c.profile = -1;

        RowOptions dep; dep.dependent = true;
        ImGui::BeginDisabled(!c.interpolation);
        SettingRow(Tr("Cuadros intermedios", "In-between frames", "Quadros intermediários"),
                   c.interpolation ? Tr("Más cuadros, más fluidez y más consumo.", "More frames, smoother motion and higher load.", "Mais quadros, mais fluidez e mais consumo.")
                                   : Tr("Activa Interpolación visual para elegir cuántos.", "Turn on Motion interpolation to choose how many.", "Ative Interpolação visual para escolher quantos."),
                   dep);
        Segmented("##frames", &c.interpolationFrames, frames, IM_ARRAYSIZE(frames));
        ImGui::EndDisabled();

        SettingRow(Tr("Filtro de texturas", "Texture filter", "Filtro de texturas"),
                   Tr("Suaviza o redibuja las texturas al ampliarlas.", "Smooths or redraws textures when upscaling them.", "Suaviza ou redesenha as texturas ao ampliá-las."));
        if (Combo("##filtro", &c.textureFilter, filters, IM_ARRAYSIZE(filters))) c.profile = -1;
        EndSettingRows();
    }

    // "###avanzado" mantiene el mismo ID (y su estado abierto/cerrado) al cambiar de idioma.
    if (CollapsibleSection(Tr("Avanzado###avanzado", "Advanced###avanzado", "Avançado###avanzado")) && BeginSettingRows("##avanzado")) {
        SettingRow(Tr("Shaders asíncronos", "Asynchronous shaders", "Shaders assíncronos"),
                   Tr("Reduce tirones la primera vez que aparece un efecto.", "Reduces stutter the first time an effect appears.", "Reduz travamentos na primeira vez que um efeito aparece."),
                   Badge_(RestartText(), Tone_Warning));
        ToggleSwitch("##async", &c.asyncShaders);

        bool rayTracing = false;
        ImGui::BeginDisabled();
        SettingRow(Tr("Trazado de rayos", "Ray tracing", "Ray tracing"),
                   Tr("No disponible: necesita una GPU compatible con Vulkan 1.3.", "Not available: requires a GPU with Vulkan 1.3 support.", "Indisponível: requer uma GPU compatível com Vulkan 1.3."));
        ToggleSwitch("##rt", &rayTracing);
        ImGui::EndDisabled();
        EndSettingRows();
    }
}

void PageCamera(DemoConfig& c) {
    SectionHeader(Tr("Vista", "View", "Visão"), true);
    if (BeginSettingRows("##vista")) {
        SettingRow(Tr("Campo de visión", "Field of view", "Campo de visão"),
                   Tr("Amplía lo que ves a los lados. Clic derecho para volver a 1.00x.", "Shows more to the sides. Right-click to reset to 1.00x.", "Amplia o que você vê dos lados. Clique direito para voltar a 1.00x."));
        SliderFloat("##fov", &c.fov, 0.8f, 1.5f, "%.2fx", 1.0f, 0.01f);
        SettingRow(Tr("Cámara libre", "Free camera", "Câmera livre"),
                   Tr("Mueve la cámara con el stick derecho. Algunos juegos no lo admiten.", "Move the camera with the right stick. Some games don't support it.", "Mova a câmera com o analógico direito. Alguns jogos não suportam."));
        ToggleSwitch("##freecam", &c.freeCamera);
        EndSettingRows();
    }
}

void PageAudio(DemoConfig& c) {
    const char* const backends[] = { Tr("Automático", "Automatic", "Automático"), "WASAPI", "XAudio2" };
    if (c.audioBackend != 0) {
        Notice(Tone_Warning, Tr("El cambio se aplicará al reiniciar el juego", "The change will apply when you restart the game", "A mudança será aplicada ao reiniciar o jogo"),
               Tr("El sonido seguirá usando el dispositivo anterior hasta entonces.", "Sound will keep using the previous device until then.", "O som continuará usando o dispositivo anterior até lá."));
        ImGui::Dummy(ImVec2(0, M::space2 * Dpi()));
    }
    SectionHeader(Tr("Volumen", "Volume", "Volume"), true);
    if (BeginSettingRows("##volumen")) {
        SettingRow(Tr("Volumen general", "Master volume", "Volume geral"));
        SliderFloat("##master", &c.volMaster, 0, 100, "%.0f %%", 80.0f, 1.0f);
        SettingRow(Tr("Música", "Music", "Música"));
        SliderFloat("##music", &c.volMusic, 0, 100, "%.0f %%", 60.0f, 1.0f);
        SettingRow(Tr("Efectos", "Effects", "Efeitos"));
        SliderFloat("##fx", &c.volEffects, 0, 100, "%.0f %%", 70.0f, 1.0f);
        EndSettingRows();
    }
    SectionHeader(Tr("Salida", "Output", "Saída"));
    if (BeginSettingRows("##salida")) {
        SettingRow(Tr("Motor de audio", "Audio backend", "Motor de áudio"),
                   Tr("Automático elige el mejor para tu equipo.", "Automatic picks the best one for your computer.", "Automático escolhe o melhor para o seu computador."),
                   Badge_(RestartText(), Tone_Warning));
        Segmented("##backend", &c.audioBackend, backends, IM_ARRAYSIZE(backends));
        EndSettingRows();
    }
}

void PageControls(DemoConfig& c) {
    Notice(Tone_Danger, Tr("No se detecta ningún mando", "No controller detected", "Nenhum controle detectado"),
           Tr("Conecta un mando por USB o Bluetooth y pulsa cualquier botón. Si ya está conectado, prueba otro puerto.",
              "Connect a controller via USB or Bluetooth and press any button. If it's already connected, try another port.",
              "Conecte um controle por USB ou Bluetooth e pressione qualquer botão. Se já estiver conectado, tente outra porta."),
           Tr("Código CTL-0042", "Code CTL-0042", "Código CTL-0042"));
    ImGui::Dummy(ImVec2(0, M::space2 * Dpi()));
    SectionHeader(Tr("Mando", "Controller", "Controle"), true);
    if (BeginSettingRows("##mando")) {
        SettingRow(Tr("Vibración", "Vibration", "Vibração"),
                   Tr("El mando vibra con los golpes y las explosiones.", "The controller rumbles with hits and explosions.", "O controle vibra com golpes e explosões."));
        ToggleSwitch("##rumble", &c.rumble);
        SettingRow(Tr("Giroscopio", "Gyro aiming", "Giroscópio"),
                   Tr("Apunta inclinando el mando. Solo mandos con sensor de movimiento.", "Aim by tilting the controller. Motion-sensor controllers only.", "Mire inclinando o controle. Apenas controles com sensor de movimento."),
                   Badge_("Beta", Tone_Neutral));
        ToggleSwitch("##gyro", &c.gyro);
        EndSettingRows();
    }
}

void PageAccessibility(DemoConfig& c) {
    const char* const themes[] = { Tr("Oscuro", "Dark", "Escuro"), Tr("Claro", "Light", "Claro") };
    const char* const scales[] = { "100 %", "125 %", "150 %", "200 %" };
    const char* const languages[] = { LanguageName(Language_Spanish), LanguageName(Language_English), LanguageName(Language_Portuguese) };
    SectionHeader(Tr("Interfaz", "Interface", "Interface"), true);
    if (BeginSettingRows("##interfaz")) {
        SettingRow(Tr("Idioma", "Language", "Idioma"),
                   Tr("Idioma de los menús. Se aplica al momento.", "Menu language. Applies right away.", "Idioma dos menus. Aplica-se na hora."));
        Segmented("##idioma", &c.language, languages, IM_ARRAYSIZE(languages));
        SettingRow(Tr("Tema", "Theme", "Tema"), Tr("Se aplica al momento.", "Applies right away.", "Aplica-se na hora."));
        Segmented("##tema", &c.theme, themes, IM_ARRAYSIZE(themes));
        SettingRow(Tr("Tamaño de interfaz", "Interface size", "Tamanho da interface"),
                   Tr("Agranda textos y controles de todos los menús.", "Makes text and controls larger in every menu.", "Aumenta textos e controles em todos os menus."));
        Segmented("##escala", &c.uiScale, scales, IM_ARRAYSIZE(scales));
        SettingRow(Tr("Subtítulos", "Subtitles", "Legendas"),
                   Tr("Muestra los diálogos como texto en pantalla.", "Shows dialogue as on-screen text.", "Mostra os diálogos como texto na tela."));
        ToggleSwitch("##subs", &c.subtitles);
        EndSettingRows();
    }
}

void PageAbout(bool* requestReset) {
    SectionHeader(Tr("Versión", "Version", "Versão"), true);
    if (BeginSettingRows("##version")) {
        SettingRow(Tr("Emulador", "Emulator", "Emulador"));
        Value("v2.4.1 · build 8f3c21a");
        SettingRow("Dear ImGui");
        Value(IMGUI_VERSION);
        SettingRow(Tr("Archivos del sistema", "System files", "Arquivos do sistema"));
        Badge(Tr("Verificada", "Verified", "Verificada"), Tone_Success);
        EndSettingRows();
    }
    ImGui::Dummy(ImVec2(0, M::space2 * Dpi()));
    Notice(Tone_Success, Tr("Tu instalación está completa", "Your installation is complete", "Sua instalação está completa"),
           Tr("Los archivos del sistema coinciden con la versión publicada.", "The system files match the published version.", "Os arquivos do sistema correspondem à versão publicada."));

    SectionHeader(Tr("Zona de peligro", "Danger zone", "Zona de perigo"));
    if (BeginSettingRows("##peligro")) {
        const char* resetAll = Tr("Restablecer todo", "Reset everything", "Redefinir tudo");
        SettingRow(resetAll, Tr("Vuelve a la configuración de fábrica. Tus partidas guardadas no se borran.",
                                "Goes back to factory settings. Your saved games are not deleted.",
                                "Volta às configurações de fábrica. Seus jogos salvos não são apagados."));
        if (DangerButton(resetAll, Icon_Trash)) *requestReset = true;
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

    // Tema, escala e idioma se aplican al momento, antes de abrir ninguna ventana (fuera de cualquier Push/Pop).
    if (baseDpi == 0.0f) {
        baseDpi = Dpi();
        lastTheme = IsDark() ? 0 : 1; lastScale = cfg.uiScale;
        cfg.theme = applied.theme = lastTheme;
        cfg.language = applied.language = (int)CurrentLanguage();
    }
    if (cfg.theme != lastTheme || cfg.uiScale != lastScale) {
        ApplyTheme(cfg.theme == 0, baseDpi * scales[cfg.uiScale]);
        lastTheme = cfg.theme; lastScale = cfg.uiScale;
    }
    SetLanguage((Language)cfg.language);

    bool requestReset = false;
    if (BeginSettingsWindow("Ajustes##ImGuiModernoDemo", open)) {
        BeginSidebar(Tr("Ajustes", "Settings", "Configurações"));
        for (int i = 0; i < Cat_COUNT; i++) {
            if (i == Cat_About) SidebarSeparator();
            ImGui::PushID(i);   // el ID no cambia con el idioma
            if (SidebarItem(CategoryName(i), category == i, kCategoryIcons[i])) category = i;
            ImGui::PopID();
        }
        EndSidebar();

        if (BeginSettingsContent(CategoryName(category))) {
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
        const char* help = Tr("Ayuda", "Help", "Ajuda");
        const char* reset = Tr("Restablecer", "Reset", "Redefinir");
        const char* apply = Tr("Aplicar", "Apply", "Aplicar");
        BeginSettingsFooter();
        GhostButton(help, Icon_Help);
        ItemTooltip(nullptr, Tr("Abre la guía del emulador en el navegador.", "Opens the emulator guide in your browser.", "Abre o guia do emulador no navegador."));
        const float gap = M::space2 * Dpi();
        SameLineRight(ButtonWidth(reset) + (dirty ? gap + ButtonWidth(apply) : 0.0f));
        if (GhostButton(reset)) { const DemoConfig keep = cfg; cfg = DemoConfig(); KeepInstant(cfg, keep); }
        if (dirty) {
            ImGui::SameLine(0, gap);
            if (PrimaryButton(apply)) applied = cfg;
        }
        EndSettingsFooter();

        // Esc cierra; si hay cambios sin aplicar, pregunta antes.
        if (open && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId) &&
            ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
            if (dirty) ImGui::OpenPopup("##cerrar");
            else *open = false;
        }
        if (requestReset) ImGui::OpenPopup("##restablecer");

        if (ConfirmModal("##cerrar",
                Tr("¿Cerrar sin aplicar los cambios?", "Close without applying changes?", "Fechar sem aplicar as alterações?"),
                Tr("Los ajustes volverán a como estaban al abrir esta ventana.", "Settings will go back to how they were when you opened this window.", "As configurações voltarão a como estavam ao abrir esta janela."),
                Tr("Cerrar sin aplicar", "Close without applying", "Fechar sem aplicar"), true,
                Tr("Seguir editando", "Keep editing", "Continuar editando")) == ConfirmResult_Confirm) {
            const DemoConfig keep = cfg; cfg = applied; KeepInstant(cfg, keep);
            if (open) *open = false;
        }
        if (ConfirmModal("##restablecer",
                Tr("¿Restablecer todos los ajustes?", "Reset all settings?", "Redefinir todas as configurações?"),
                Tr("Se perderá tu configuración. Tus partidas guardadas no se verán afectadas.", "Your settings will be lost. Your saved games won't be affected.", "Suas configurações serão perdidas. Seus jogos salvos não serão afetados."),
                Tr("Restablecer todo", "Reset everything", "Redefinir tudo")) == ConfirmResult_Confirm) {
            const DemoConfig keep = cfg; cfg = applied = DemoConfig(); KeepInstant(cfg, keep); KeepInstant(applied, keep);
        }
    }
    EndSettingsWindow();
}

} // namespace ImGuiModerno
