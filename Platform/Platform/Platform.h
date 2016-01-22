#ifndef Platform_h__
#define Platform_h__


namespace Platform
{

  struct Component
  {
    virtual void LoadMesh(char* path) = 0;
    virtual void LoadShader(char* path) = 0;
    virtual void LoadSound(char* path) = 0;
    virtual void LoadImage(char* path) = 0;
    virtual void SetNumber(double value) = 0;
    virtual void SetText(char* text) = 0;
    virtual double GetNumber() = 0;
    virtual char* GetText() = 0;
  };

  struct Entity
  {
    virtual void AddComponent(Component* scene) = 0;
    Component* AddComponent(char* name);
  };

  struct Actor
  {
    virtual void AddEntity(Entity* scene) = 0;
    Entity* AddEntity(char* name);
  };

  struct Scene
  {
    virtual void AddActor(Actor* scene) = 0;
    Actor* AddActor(char* name);
  };

  struct Game
  {
    virtual void AddScene(Scene* scene) = 0;
    Scene* AddScene(char* name);
    virtual void BeginGame() = 0;
    virtual void SetDisplayMode(char* Title, int width, int height, bool fullScreen) = 0;
  };

  Component* CreateComponent(char* name);
  Entity* CreateEntity(char* name);
  Actor* CreateActor(char* name);
  Scene* CreateScene(char* name);
  Game* CreateGame(char* name);

}
#endif // Platform_h__