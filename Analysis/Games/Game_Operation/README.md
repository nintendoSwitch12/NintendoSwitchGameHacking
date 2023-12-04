# 스플래툰 게임 동작 방법


Thisanalysis was conducted in Splatoon 2



# Overview


In generals, games calls functions performing the following functionalities within the main loop.

- process input
- update game (event)
- render

![Untitled](./img/1.png)

This article briefly describes the process leading to the main loop in Nintendo Switch games and outlines the operation of Switch game mechanics.

## Reasons for the analysis


Below are the reasons why we analyzed the workings of the game.


1. To make the analysis of other games more convenient:
    1. It allows for creating an overall picture of how games run on the Switch.
    2. Since all games operate similarly, this would make the analysis process more streamlined.
        1. It provides insights into how multi-threading functions in games.
2. Theoretically, it is possible to analyze every functions used in the game by analyzing the main loop.
    1. If specific functions are hard to locate, main loop can be used to trace each function.  

# Exploring the main loop



## Before nnMain



This part is not included in the binary but it's supposed to be a routine to call nnMain.

```
v31 = ((__int64 (__fastcall *)(__int64 *, const char *))unk_80000750)(
                    v30,
                    "_ZN2nn2ro6detail15g_pAutoLoadListE");
if ( v31 && *(unsigned __int8 *)(v31 + 4) >= 0x10u )
  *(_QWORD *)(*(_QWORD *)(v31 + 8) + v30[4]) = &qword_800053E0;
v32 = ((__int64 (__fastcall *)(__int64 *, const char *))unk_80000750)(
        v30,
        "_ZN2nn2ro6detail17g_pManualLoadListE");
if ( v32 && *(unsigned __int8 *)(v32 + 4) >= 0x10u )
  *(_QWORD *)(*(_QWORD *)(v32 + 8) + v30[4]) = qword_800053D0;
v33 = ((__int64 (__fastcall *)(__int64 *, const char *))unk_80000750)(
        v30,
        "_ZN2nn2ro6detail14g_pRoDebugFlagE");
if ( v33 && *(unsigned __int8 *)(v33 + 4) >= 0x10u )
  *(_QWORD *)(*(_QWORD *)(v33 + 8) + v30[4]) = 21488LL;
v34 = ((__int64 (__fastcall *)(__int64 *, const char *))unk_80000750)(
        v30,
        "_ZN2nn2ro6detail34g_pLookupGlobalAutoFunctionPointerE");
if ( v34 && *(unsigned __int8 *)(v34 + 4) >= 0x10u )
  *(_QWORD *)(*(_QWORD *)(v34 + 8) + v30[4]) = 1684LL;
result = ((__int64 (__fastcall *)(__int64 *, const char *))unk_80000750)(
           v30,
           "_ZN2nn2ro6detail36g_pLookupGlobalManualFunctionPointerE");
if ( result && *(unsigned __int8 *)(result + 4) >= 0x10u )
  *(_QWORD *)(*(_QWORD *)(result + 8) + v30[4]) = 21496LL;
```

## nnMain ~ main loop 까지과정


All applications on Nintedno Switch begins with the function nnMain. Therefore, all functions can be searched from nnMain in a top-down manner.

A typical nnMain has the following structure.

```
void __fastcall __noreturn nnMain(Lp::Sys::SysInit *this, const char *a2)
{
  *(_QWORD *)off_82FA6FC0 = &off_82D82120;
  Lp::Sys::SysInit::Init(this, a2);
}
```

In the function above, various settings are initialized through the function Lp::SysInit::SysInit::Init, and lead:Framework::run is called.

```
Lp::Sys::SysInit::Init(const char* x)
{
// ...
	return framework->sead::Framework::run(framework, ?, ?, ?)
}
```

From seed::Framework::run, seed::GameFrameworkNx::runImpl_ is called.

```
sead::Framework::run(?, ?, ?)
{
// ...
	sead::GameFrameworkNx::runImpl_();
	return sead::GameFramework::quitRun_();
}
```

Below is a the function of seed::GameFrameworkNx::runImpl_ .  It calls sead::GameFramework::waitStartDisplayLoop_ which is responsible for display related details. 

```
sead::GameFrameworkNx::runImpl_(void)
{
// ...
  v2 = sead::GameFramework::waitStartDisplayLoop_();
// ...
  return sead::GameFrameworkNx::mainLoop_();
}
```

After the above function, sead::GameFrameworkNx::mainLoop_ is called which is the main loop function.

```
sead::GameFrameworkNx::mainLoop_(void)
{
  while ( 1 )
    Lp::Sys::Framework::procFrame_();
}
```

## Summary Illustration


![Untitled](./img/2.png)

# main loop(Thread) analysis

The Lp::Sys::Framework::procFrame_ function that is called in the main loop calls sead::GameFrameNx::procFrame_ function which looks like below. 

```
sead::GameFrameworkNx::procFrame_(void)
{
// ...
  sead::TaskMgr::afterCalc(task_manager_);
  sead::GameFrameworkNx::procDraw_();
  sead::GameFrameworkNx::procCalc_();
  sead::Framework::procReset_();
  v3 = sead::GameFrameworkNx::waitForGpuDone_();
// ...
}
```

The functions called by the Lp::Sys::Framework::procFrame_ function, including the Lp::Sys::Frame_ function, are as follows.

- Lp::Sys::Framework::procFrame_ → calls the function (sead::GameFrameworkNx::procFrame_) that updates the game every frame.
    - sead::GameFrameworkNx::procFrame_ → updates the game every frame.
- sead::TaskMgr::afterCalc → after completing all tasks, proceeds with post-processing.
- sead::GameFrameworkNx::procDraw_ → creates a task that performs rendering.
- sead::GameFrameworkNx::procCalc_ → performs the actual tasks.
    - sead::TaskMgr::beforeCalc → creates a task.
- sead::Framework::procReset_ → initializes the task.

Except for rendering, the operation of each function is as follows.

## sead::TaskMgr::afterCalc

This function immediately calls seed::TaskMgr::calcDestruction_. The lead:TaskMgr::calcDestruction_ function traverses the Task list, and removes the task from the list when the task is finished. And get rid of all the tasks.

## sead::GameFrameworkNx::procCalc_


This function creates a task through the lead::TaskMgr::beforeCalc function. And invoke each Delegate through the lead::MethodTreeNode::call function. In other words, it is a function that allows you to perform an actual task.


The following are the delegates called in this function.

- sead::Delegate<Lp::Net::EnlTask>
- sead::Delegate<Lp::Sys::DbgCameraMgr>
- sead::Delegate<Lp::UI::UIMgr>
- sead::Delegate<gsys::SystemTask>
- sead::Delegate<sead::CalculateTask>
- sead::Delegate<sead::MethodTreeNode>

Here, the imported tasks are performed in Main Thread. In addition, this function also calls the seed::FixedSizeJQ::enque function and the seed::FixedSizeJQ::run function. The lead::FixedSizeJQ::enque function adds a job to the JobQue, and the lead::FixedSizeJQ::run function performs functions in the JobQueue.

## sead::Framework::procReset_


After waiting for all tasks to complete, delete all tasks and add a new Root Task.

## 요약 그림


![Untitled](./img/3.png)

# Main thread의 Delegate 분석

Delegate can be used in a multi-threading environment and can be used in callback, event-driven multi-threading implementation, etc. The following are the types of Delegates used by the Nintendo Switch application Main Thread.

- Lp::Net::EnlTask
- Lp::Sys::DbgCameraMgr
- Lp::UI::UIMgr
- gsys::SystemTask
- sead::CalculateTask
- sead::MethodTreeNode

In this sector, functions called by each type of Delegate are briefly analyzed.

## sead::Delegate<Lp::Net::EnlTask>


The following is a function called through Lp::Net:EnlTask Delegate.

- Lp::Net::EnlTask::calcAfterScene


This is Lp::Net::EnlTask::calcAfterScene.

```
__int64 Lp::Net::EnlTask::calcAfterScene()
{
  default_content_transport_manager_->Lp::Net::ContentTransporterMgrBase::calcAfterScene(default_content_transport_manager_);
  Lp::Net::PiaCloneMgr::calcSend(*Lp::Net::PiaCloneMgr::sInstance);
  Lp::Net::PiaChatMgr::calcMainThread(*Lp::Net::PiaChatMgr::sInstance);
  enl::Framework::calcSend(this->framework_);
  return default_matching_sequence_->Lp::Net::DefaultMatchingSeq::calcAfterScene(default_matching_sequence_);
}
```

## sead::Delegate<gsys::SystemTask>


- gsys::SystemTask::postDrawTV_
- gsys::SystemTask::postCalc_
- gsys::SystemTask::preCalc_
- gsys::SystemTask::drawTV_

Functions called in this Delegate are functions related to graphic rendering.

gsys is presumed to be an abbreviation for graphic system

## sead::Delegate<sead::CalculateTask>


- Lp::Sys::CtrlMgr::calc
- Cmn::BcatMgr::calc
- Lp::Sys::DbgMode::calc
- Lp::Sys::XLinkMgr::calc
- Cmn::DbgMapVisitor::calc
- Cmn::SceneLoadDispMgr::calc
- Cmn::OEFocusHandlingModeChanger::calc
- Cmn::SoundMgr::calc
- Lp::Net::EnlTask::calc
- Lp::Sys::MemProfiler::calc
- Lp::Sys::ProcessSwitch::calc
- sead::ControllerMgr::calc
- Cmn::PBRPrepareTask::calc
- Cmn::CoconutMgr::calc
- Lp::Sys::SaveDataMgr::calc
- Cmn::DayChangeChecker::calc
- Lp::Sys::Nfp::calc
- Lp::Sys::WarningMgr::calc
- Lp::Sys::LuaMgr::calc
- Cmn::Pause::calc
- Lp::Sys::RootTask::calc
- Lp::UI::UIMgr::calc
- Lp::Sys::TimeStamp::calc
- Cmn::SceneBasePostGsysCalcTask::calc
- Cmn::CtrlChecker::calc
- Lp::Sys::DbgTextWriter::calc
- Cnet::NexMaintenanceChecker::calc
- Lp::Sys::RumbleMgr::calc
- Lp::Sys::PauseMgr::calc
- Lp::Sys::SoundMgr::calc
- Lp::Sys::FuncForShow::calc
- Cmn::GPUFootPrintCalcForProduct::calc
- Lp::Sys::SceneMgr::calc
- Cmn::OekakiMgr::calc
- Lp::Sys::EffectMgr::calc

## sead::Delegate<sead::MethodTreeNode>


No function is called through sead::MethodTreeNode.

## sead::Delegate<Lp::Sys::DbgCameraMgr>


No function is called through Lp::Sys::DbgCameraMgrDelegate.

## sead::Delegate<Lp::UI::UIMgr>


No function is called through Lp::UI::UIMgr.

# JobQueue & WorkerMgr/Worker Thread

Most games that support Multi-Threading implement Multi-Threading through the Job (Task) System. The Job System puts tasks to be performed in a queue, and each Thread performs tasks in the Job(Task) queue through the ead::FixedSizeJQ::run function.


The following is a list of threads that exist on the Nintendo Switch.

- AppletThread
- Bcat
- LibcurlResolver
- Lp::Sys::VibrationThread
- Nfp
- Pia BackgroundScheduler
- Prepare Thread
- Presentation Thread
- SaveDataMgr
- SceneGeneralMain
- SceneGeneralSub1
- SceneGeneralSub2
- SceneLoad
- SceneLoadInCalc
- SoundCalcCallerAsLongBlock
- Thread 72
- Thread 79
- UIJob
- UpdateTextureCacheThread
- VibrationThread
- WorkerMgr/Worker1(?)
- WorkerMgr/Worker2(?)
- aal::LowPrioWorkerThread
- enl::TaskThread
- nn::atk::detail::TaskThread
- nn::atk::detail::driver::SoundThread

If there is a name for Thread on the Nintendo Switch, perform the appropriate task for that name. However, in the case of Worker Thread, the following tasks are performed.

- Lp::Sys::CalcInGSysCalc::CalcInGSysCalc_DefaultJob::invoke
- agl::lyr::LayerJob::invoke
- gsys::ModelJobQueue::Job::invoke:
- gsys::ModelRenderQueue::invoke
- gsys::ModelScene::Job::invoke
- gsys::ParticleMgr::ParticleCalcJob::invoke
- gsys::SystemJob::invoke
- sead::Job0<Lp::Sys::Actor>::invoke

gsys, LayerJob are graphics related tasks so ignore them.

Then there are two functions left.

- sead::Job0<Lp::Sys::Actor>::invoke


The lead:Job0<Lp::Sys::Actor>::invoke function continues to execute the function as in the image.

- Lp::Sys::Actor::actorSysCalc

그림으로 표현하면 다음과 같다.

![Untitled](./img/4.png)

## Actor


Actor means that it is related to an object that can be controlled by a player or AI. In other words, it can be seen that the Lp::Sys::Actor::factorSysCalc function found above is a function that performs operations related to these entities.This function calls three functions.

- Cmn::Actor::actorCalc
- Lp::Sys::Actor::actorSysCalc
- Cmn::Actor::actorCalc_PostChildCalcByThis

The important function here is the Cmn::Actor::factorCalc function.

```
Lp::Sys::Actor* Cmn::Actor::actorCalc(int step)
{
// ...
  switch ( step )
  {
    case 0:
      Game::NpcBase::firstCalc();
      if ( component_holder_ )
      {
        component_holder_->Cmn::Actor::actorDestroy();
      }
      break;
    case 1:
      Game::Actor::secondCalc();
      if ( component_holder_ )
      {
        component_holder_->Cmn::Actor::actorCalc();
      }
      break;
    case 2:
      Game::NpcBase::thirdCalc();
      if ( component_holder_ )
      {
        component_holder_->Cmn::Actor::actorCalc_PostChildCalcByThis();
      }
      break;
    case 3:
      if ( component_holder_ )
        component_holder_->Cmn::ComponentHolder::preFourthCalc(component_holder);
      Game::NpcBase::fourthCalc();
      if ( component_holder_ )
      {
        component_holder_->Cmn::Actor::actorDbgDraw2D();
      }
      break;
    case 4:
      component_holder_->Cmn::Actor::fifthCalc();
      break;
    case 5:
      this = (Lp::Sys::Actor *)this->vtable_->Cmn::Actor::isCalcXLink(this);

      if ( ((Cmn::Actor::isCalcXLink() & 1) != 0 )
      {
        Cmn::Actor::xlinkCalc();
        if ( x_link_ )
        {
          x_link_->Lp::Sys::XLink::calc();
        }
      }
      break;
    default:
      return this;
  }
  return this;
}
```

This function performs operations related to interactable objects according to the steps. Therefore, this function performs operations such as damage calculation.