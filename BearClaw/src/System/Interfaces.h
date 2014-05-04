#ifndef INTERFACES_H
#define INTERFACES_H

#include <System/OS/PlatformIncludes.h>

namespace BearClaw {

//Typedefs for shared pointers
class SceneNode;
typedef std::shared_ptr<SceneNode> StrongNodePtr;
typedef std::weak_ptr<SceneNode> WeakNodePtr;

class Component;
typedef std::shared_ptr<Component> StrongComponentPtr;
typedef std::weak_ptr<Component> WeakComponentPtr;

class RenderNode;
typedef std::shared_ptr<RenderNode> StrongRenderNodePtr;
typedef std::weak_ptr<RenderNode> WeakRenderNodePtr;
//

}
#endif
