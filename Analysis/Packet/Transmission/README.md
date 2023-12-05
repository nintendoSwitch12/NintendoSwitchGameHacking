# Packet Send & Receive through IPC

## Outline

Nintendo Switch uses IPC to send and receive packets. This articles talks about how this process is carried out. 

## System calls that uses IPC

There are 3 syscalls that send requests to Nintendo services using IPC.

- svc::SendSyncRequest()
- svc::SendSyncRequestWithUserBuffer()
- svc::SendAsyncRequestWithUserBuffer()

We only looked at svc::SendSyncRequest() since it was the only syscall that sent the packet during our analysis. However, other syscalls may be used in other situations. 

## Using Message Buffer


Before sending packets, the data to be sent is stored in a message buffer. This location is then referenced to be sent to the target by calling svc::SendSyncRequest().

Below shows how the message buffer is referenced. 

Analysis

Below is a code fragment executing svc::SendSyncRequest() when the if statement is true.

![Untitled](./img/1.png)

The variable a2 is located in thread local storage which is used when communicating with the OS using IPC. 

In addition, ARM64_SYSREG(3, 3, 13, 0, 3) is defined as below.

![Untitled](./img/2.png)

TPIDR_EL0 stores per-thread structure and again we know a2 is in thread local storage.[1] 

In summary, before sending a request to IPC, the game checks if the data to be sent is located in thread local storage. If so, svc::SendSyncRequest is called, transferring the data through IPC and sending a packet using one of Nintendo’s services. 

## System calls used when sending/receiving packets

svc 0x21 - SendSyncRequest(Handle session_handle)

Sends message buffers(TLS) through IPC to transfer a packet to the server. 

svc 0x43 - ReplyAndReceive(int32_t *out_index, const Handle *handles, int32_t num_handles, Handle reply_target, int64_t timeout_ns)

Waits for packet to be received and write the response packet in the message buffer(TLS) address received. 

## Links


[1] [https://stackoverflow.com/questions/64856566/what-is-the-purpose-of-thread-id-registers-like-tpidr-el0-tpidr-el1-in-arm](https://stackoverflow.com/questions/64856566/what-is-the-purpose-of-thread-id-registers-like-tpidr-el0-tpidr-el1-in-arm)