#ifndef RENDERER_H
#define RENDERER_H

#include <Renderer/RenderScene.h>
#include <Utils/ConfigSet.h>
#include <Scene/Scene.h>
#include <Renderer/Utils/HLShaderWrapper.h>
#include <Gui/FontString.h>

namespace BearClaw {
class RendererInitializer : ConfigSet
{
public:
    RendererInitializer();
    void SetValue(const char* Name, f64 Value) {Set(Name, Value);};
};



class Renderer
{
private:

protected:
    RendererInitializer m_Settings;
    RenderScene*        m_RenderScene;
    bool                m_SceneHasBeenSet;

public:
    Renderer(RendererInitializer Initializer);
    ~Renderer();
    virtual void Render();

    GLuint ModelViewMatrixLoc;
    GLuint ProjectionMatrixLoc;
    GLuint NormalMatrixLoc;

    void Init();
    void SetupShaders();

    void SetScene(RenderScene* Scn)
    {
        m_RenderScene = Scn;
        m_SceneHasBeenSet = true;
    }

	void UnsetScene() {
		m_SceneHasBeenSet = false;
		m_RenderScene = nullptr;
	}

    RenderScene* GetRenderScene()
    {
        return m_RenderScene;
    }

    void SetSettingValue(const char* Name, f64 Value)
    {
        m_Settings.SetValue(Name, Value);
    }

    bool DebugDrawEnabled() { return true; }
};
}

#endif
