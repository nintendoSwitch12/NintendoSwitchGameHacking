# enl task thread분석

모든 분석은 스플래툰2에서 수행되었습니다.

# Packet receive

![sdfasdfasdf.png](./img/1.png)

# **ENL TASK THREAD**

실질적으로 ENL 관련 처리 Thread

```jsx
sead::Thread::run_ → messageQueuepop -> ...
-> enl::TaskThread::calc_ -> enl::PiaFramework::dispatch → *
```

위와 같은 순서로 실행이 되며, 실질적으로 패킷을 처리한다.

```jsx
nn::pia::transport::ProtocolManager::Dispatch 
-> nn::pia::transport::StationProtocol::Dispatch
-> nn::pia::transport::StationProtocol::ParseMsgFromReliable 
-> nn::pia::transport::StationProtocolReliable::Receive
```

예를 들자면, 방에 접속할때는 아래와 같이 처리된다.

```jsx
nn::pia::transport::ConnectStationJob::SendConnectionRequest
-> nn::pia::transport::ConnectStationJob::SendConnectionRequestMessage 
-> nn::pia::transport::StationProtocol::MakeConnectionRequestData 
-> ( nn::pia::transport::StationConnectionInfo::GetSerializedSize = 0x46 sz ) & ( nn::pia::transport::StationConnectionInfo::Serialize) 
-> nn::pia::transport::StationLocation::Serialize (Little <-> Big)
```

### ****unreliable protocol session search****

1-day 분석했던 unreliable protocol도 이 쓰레드에서 처리되었던 프로토콜이다.
다른 쓰레드에서 요청이 들어올때 처리되는 경우가 있다.

```jsx
... -> sead::Thread::run_ -> enl::TaskThread::calc_ 
-> sead::Delegate1R<enl::MatchingManager,void *,enl::Result>::invoke 
-> enl::MatchingManager::callfindMatch -> enl::PiaLocalMatchingManager::findMatch 
->  enl::PiaLocalMatchingManager::matchmake_
```

이때 matchmake 내부에서 호출되는 함수중 enl::PiaLocalMatchingManager::prepareSession_는 세션 초기화를 해주고 enl::PiaLocalMatchingManager::procSessionEvent_가 케이스별로  enl::PiaUtil::JoinSession & enl::PiaUtil::SearchSession & enl::PiaUtil::CreateSession 함수를 호출한다.

이때 세션을 검색하는 부분인데, 그때의 함수 호출은 아래와 같이 이루어진다.

```jsx
enl::PiaUtil::SearchSession -> enl::PiaFramework::dispatch 
-> nn::pia::common::Scheduler::Dispatch -> nn::pia::common::Job::Execute 
-> nn::pia::transport::Transport::DispatchJob::ExecuteCore 
-> nn::pia::transport::Transport::dispatch 
-> nn::pia::transport::ProtocolManager::Dispatch 
-> nn::pia::transport::UnreliableProtocol::Dispatch
```

## **Pia class Serialize & Deserialize**

```c
_BYTE *__fastcall nn::pia::common::serializeU64(_BYTE *this, unsigned __int8 *a2)
{
  *this = HIBYTE(a2);
  this[1] = BYTE6(a2);
  this[2] = BYTE5(a2);
  this[3] = BYTE4(a2);
  this[4] = BYTE3(a2);
  this[5] = BYTE2(a2);
  this[6] = BYTE1(a2);
  this[7] = (_BYTE)a2;
  return this;
}
unsigned __int64 __fastcall nn::pia::common::deserializeU64(nn::pia::common *this, const unsigned __int8 *a2)
{
  return _byteswap_uint64(*(_QWORD *)this);
}

```

Little <-> Big Endian 변환 작업도 수행한다.