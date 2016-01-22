#include <vector>

struct _Actor
{
  void* actor;
  void(*Update)(void*);

  _Actor(Actor* act, void(*update)(void*))
  {
    actor = act;
    Update = update;
  }
};


struct Actor
{
  virtual void Update(void*) = 0;
};

struct SubActor
{
  int x;
  //Components

  SubActor()
  {
    x = 1;
  }

  virtual void Update(void*)
  {
    x = 5;
  }
};

struct ActorBluePrint
{
  Actor*(*ActorCreator)();
  void(*Update)(void*);
};

template <typename T>
void ActorUpdate(void* actor)
{
  T* a = (T*)actor;
  a->Update();
}

template <typename T>
void* ConstructActor()
{
  return memnew(T)();
}

struct Engine
{
  std::vector<ActorBluePrint> bluePrints;
  std::vector<_Actor> actors;

  template <typename T>
  void AddActorBluePrint()
  {
    ActorBluePrint blueprint;
    blueprint.ActorCreator = &ConstructActor<T>;
    blueprint.Update = &ActorUpdate<T>;
    bluePrints.push_back(blueprint);
  }

  void CreateActor(int index)
  {
    actors.emplace_back(bluePrints[index].ActorCreator(), bluePrints[index].Update);
  }
};

void Test()
{
  Engine engine;
  engine.AddActorBluePrint<SubActor>();
  engine.CreateActor(0);

  engine.actors[0].Update(&(engine.actors[0].actor));
}








struct Behavior
{
  
  void update(){};
  void (*) update(actor){};
  Entity* list;

  private create();
  private setEntities()
  {

  }
}

struct chickenBehavior : Behavior
{

};