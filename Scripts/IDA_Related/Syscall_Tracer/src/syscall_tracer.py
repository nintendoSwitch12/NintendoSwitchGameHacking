import idautils
import idc
import ida_bytes
import re
import ida_funcs
import idaapi
import ida_dbg

a = '''0x01     Result  SetHeapSize     uintptr_t *out_address, size_t size
0x02    Result  SetMemoryPermission     uintptr_t address, size_t size, MemoryPermission perm
0x03    Result  SetMemoryAttribute      uintptr_t address, size_t size, uint32_t mask, uint32_t attr
0x04    Result  MapMemory       uintptr_t dst_address, uintptr_t src_address, size_t size
0x05    Result  UnmapMemory     uintptr_t dst_address, uintptr_t src_address, size_t size
0x06    Result  QueryMemory     arch::MemoryInfo *out_memory_info, PageInfo *out_page_info, uintptr_t address
0x07    void    ExitProcess
0x08    Result  CreateThread    Handle *out_handle, ThreadFunc func, uintptr_t arg, uintptr_t stack_bottom, int32_t priority, int32_t core_id
0x09    Result  StartThread     Handle thread_handle
0x0A    void    ExitThread
0x0B    void    SleepThread     int64_t ns
0x0C    Result  GetThreadPriority       int32_t *out_priority, Handle thread_handle
0x0D    Result  SetThreadPriority       Handle thread_handle, int32_t priority
0x0E    Result  GetThreadCoreMask       int32_t *out_core_id, uint64_t *out_affinity_mask, Handle thread_handle
0x0F    Result  SetThreadCoreMask       Handle thread_handle, int32_t core_id, uint64_t affinity_mask
0x10    int32_t         GetCurrentProcessorNumber
0x11    Result  SignalEvent     Handle event_handle
0x12    Result  ClearEvent      Handle event_handle
0x13    Result  MapSharedMemory         Handle shmem_handle, uintptr_t address, size_t size, MemoryPermission map_perm
0x14    Result  UnmapSharedMemory       Handle shmem_handle, uintptr_t address, size_t size
0x15    Result  CreateTransferMemory    Handle *out_handle, uintptr_t address, size_t size, MemoryPermission map_perm
0x16    Result  CloseHandle     Handle handle
0x17    Result  ResetSignal     Handle handle
0x18    Result  WaitSynchronization     int32_t *out_index, const Handle *handles, int32_t numHandles, int64_t timeout_ns
0x19    Result  CancelSynchronization   Handle handle
0x1A    Result  ArbitrateLock   Handle thread_handle, uintptr_t address, uint32_t tag
0x1B    Result  ArbitrateUnlock         uintptr_t address
0x1C    Result  WaitProcessWideKeyAtomic        uintptr_t address, uintptr_t cv_key, uint32_t tag, int64_t timeout_ns
0x1D    void    SignalProcessWideKey    uintptr_t cv_key, int32_t count
0x1E    int64_t         GetSystemTick
0x1F    Result  ConnectToNamedPort      Handle *out_handle, const char *name
0x20    Result  SendSyncRequestLight    Handle session_handle
0x21    Result  SendSyncRequest         Handle session_handle
0x22    Result  SendSyncRequestWithUserBuffer   uintptr_t message_buffer, size_t message_buffer_size, Handle session_handle
0x23    Result  SendAsyncRequestWithUserBuffer  Handle *out_event_handle, uintptr_t message_buffer, size_t message_buffer_size, Handle session_handle
0x24    Result  GetProcessId    uint64_t *out_process_id, Handle process_handle
0x25    Result  GetThreadId     uint64_t *out_thread_id, Handle thread_handle
0x26    void    Break   BreakReason break_reason, uintptr_t arg, size_t size
0x27    Result  OutputDebugString       const char *debug_str, size_t len
0x28    void    ReturnFromException     Result result
0x29    Result  GetInfo         uint64_t *out, InfoType info_type, Handle handle, uint64_t info_subtype
0x2A    void    FlushEntireDataCache
0x2B    Result  FlushDataCache  uintptr_t address, size_t size
[3.0.0+] 0x2C   Result  MapPhysicalMemory       uintptr_t address, size_t size
[3.0.0+] 0x2D   Result  UnmapPhysicalMemory     uintptr_t address, size_t size
[5.0.0-5.1.0] 0x2E      Result  GetFutureThreadInfo     arch::LastThreadContext *out_context, uintptr_t *out_tls_address, uint32_t *out_flags, int64_t ns
[6.0.0+] 0x2E   Result  GetDebugFutureThreadInfo        arch::LastThreadContext *out_context, uint64_t *thread_id, Handle debug_handle, int64_t ns
0x2F    Result  GetLastThreadInfo       arch::LastThreadContext *out_context, uintptr_t *out_tls_address, uint32_t *out_flags
0x30    Result  GetResourceLimitLimitValue      int64_t *out_limit_value, Handle resource_limit_handle, LimitableResource which
0x31    Result  GetResourceLimitCurrentValue    int64_t *out_current_value, Handle resource_limit_handle, LimitableResource which
0x32    Result  SetThreadActivity       Handle thread_handle, ThreadActivity thread_activity
0x33    Result  GetThreadContext3       ThreadContext *out_context, Handle thread_handle
[4.0.0+] 0x34   Result  WaitForAddress  uintptr_t address, ArbitrationType arb_type, int32_t value, int64_t timeout_ns
[4.0.0+] 0x35   Result  SignalToAddress         uintptr_t address, SignalType signal_type, int32_t value, int32_t count
[8.0.0+] 0x36   void    SynchronizePreemptionState
[11.0.0+] 0x37  Result  GetResourceLimitPeakValue       int64_t *out_peak_value, Handle resource_limit_handle, LimitableResource which
[13.0.0+] 0x39  Result  CreateIoPool    Handle *out_handle, IoPoolType which_pool
[13.0.0+] 0x3A  Result  CreateIoRegion  Handle *out_handle, Handle io_pool, PhysicalAddress physical_address, size_t size, MemoryMapping mapping, MemoryPermission perm
[1.0.0-3.0.2] 0x3C      void    DumpInfo        DumpInfoType dump_info_type, uint64_t arg
[4.0.0+] 0x3C   void    KernelDebug     KernelDebugType kern_debug_type, uint64_t arg0, uint64_t arg1, uint64_t arg2
[4.0.0+] 0x3D   void    ChangeKernelTraceState  KernelTraceState kern_trace_state
0x40    Result  CreateSession   Handle *out_server_session_handle, Handle *out_client_session_handle, bool is_light, uintptr_t name
0x41    Result  AcceptSession   Handle *out_handle, Handle port
0x42    Result  ReplyAndReceiveLight    Handle handle
0x43    Result  ReplyAndReceive         int32_t *out_index, const Handle *handles, int32_t num_handles, Handle reply_target, int64_t timeout_ns
0x44    Result  ReplyAndReceiveWithUserBuffer   int32_t *out_index, uintptr_t message_buffer, size_t message_buffer_size, const Handle *handles, int32_t num_handles, Handle reply_target, int64_t timeout_ns
0x45    Result  CreateEvent     Handle *out_write_handle, Handle *out_read_handle
[13.0.0+] 0x46  Result  MapIoRegion     Handle io_region, uintptr_t address, size_t size, MemoryPermission perm
[13.0.0+] 0x47  Result  UnmapIoRegion   Handle io_region, uintptr_t address, size_t size
[5.0.0+] 0x48   Result  MapPhysicalMemoryUnsafe         uintptr_t address, size_t size
[5.0.0+] 0x49   Result  UnmapPhysicalMemoryUnsafe       uintptr_t address, size_t size
[5.0.0+] 0x4A   Result  SetUnsafeLimit  size_t limit
[4.0.0+] 0x4B   Result  CreateCodeMemory        Handle *out_handle, uintptr_t address, size_t size
[4.0.0+] 0x4C   Result  ControlCodeMemory       Handle code_memory_handle, CodeMemoryOperation operation, uint64_t address, uint64_t size, MemoryPermission perm
0x4D    void    SleepSystem
0x4E    Result  ReadWriteRegister       uint32_t *out_value, PhysicalAddress address, uint32_t mask, uint32_t value
0x4F    Result  SetProcessActivity      Handle process_handle, ProcessActivity process_activity
0x50    Result  CreateSharedMemory      Handle *out_handle, size_t size, MemoryPermission owner_perm, MemoryPermission remote_perm
0x51    Result  MapTransferMemory       Handle trmem_handle, uintptr_t address, size_t size, MemoryPermission owner_perm
0x52    Result  UnmapTransferMemory     Handle trmem_handle, uintptr_t address, size_t size
0x53    Result  CreateInterruptEvent    Handle *out_read_handle, int32_t interrupt_id, InterruptType interrupt_type
0x54    Result  QueryPhysicalAddress    arch::PhysicalMemoryInfo *out_info, uintptr_t address
0x55    Result  QueryIoMapping  uintptr_t *out_address, [10.0.0+] size_t *out_size, PhysicalAddress physical_address, size_t size
0x56    Result  CreateDeviceAddressSpace        Handle *out_handle, uint64_t das_address, uint64_t das_size
0x57    Result  AttachDeviceAddressSpace        DeviceName device_name, Handle das_handle
0x58    Result  DetachDeviceAddressSpace        DeviceName device_name, Handle das_handle
0x59    Result  MapDeviceAddressSpaceByForce    Handle das_handle, Handle process_handle, uint64_t process_address, size_t size, uint64_t device_address, uint32_t option
0x5A    Result  MapDeviceAddressSpaceAligned    Handle das_handle, Handle process_handle, uint64_t process_address, size_t size, uint64_t device_address, uint32_t option
[1.0.0-12.1.0] 0x5B     Result  MapDeviceAddressSpace   size_t *out_mapped_size, Handle das_handle, Handle process_handle, uint64_t process_address, size_t size, uint64_t device_address, MemoryPermission device_perm
0x5C    Result  UnmapDeviceAddressSpace         Handle das_handle, Handle process_handle, uint64_t process_address, size_t size, uint64_t device_address
0x5D    Result  InvalidateProcessDataCache      Handle process_handle, uint64_t address, uint64_t size
0x5E    Result  StoreProcessDataCache   Handle process_handle, uint64_t address, uint64_t size
0x5F    Result  FlushProcessDataCache   Handle process_handle, uint64_t address, uint64_t size
0x60    Result  DebugActiveProcess      Handle *out_handle, uint64_t process_id
0x61    Result  BreakDebugProcess       Handle debug_handle
0x62    Result  TerminateDebugProcess   Handle debug_handle
0x63    Result  GetDebugEvent   arch::DebugEventInfo *out_info, Handle debug_handle
0x64    Result  ContinueDebugEvent      Handle debug_handle, uint32_t flags, const uint64_t *thread_ids, int32_t num_thread_ids
0x65    Result  GetProcessList  int32_t *out_num_processes, uint64_t *out_process_ids, int32_t max_out_count
0x66    Result  GetThreadList   int32_t *out_num_threads, uint64_t *out_thread_ids, int32_t max_out_count, Handle debug_handle
0x67    Result  GetDebugThreadContext   ThreadContext *out_context, Handle debug_handle, uint64_t thread_id, uint32_t context_flags
0x68    Result  SetDebugThreadContext   Handle debug_handle, uint64_t thread_id, const ThreadContext *context, uint32_t context_flags
0x69    Result  QueryDebugProcessMemory         arch::MemoryInfo *out_memory_info, PageInfo *out_page_info, Handle process_handle, uintptr_t address
0x6A    Result  ReadDebugProcessMemory  uintptr_t buffer, Handle debug_handle, uintptr_t address, size_t size
0x6B    Result  WriteDebugProcessMemory         Handle debug_handle, uintptr_t buffer, uintptr_t address, size_t size
0x6C    Result  SetHardwareBreakPoint   HardwareBreakPointRegisterName name, uint64_t flags, uint64_t value
0x6D    Result  GetDebugThreadParam     uint64_t *out_64, uint32_t *out_32, Handle debug_handle, uint64_t thread_id, DebugThreadParam param
[5.0.0+] 0x6F   Result  GetSystemInfo   uint64_t *out, SystemInfoType info_type, Handle handle, uint64_t info_subtype
0x70    Result  CreatePort      Handle *out_server_handle, Handle *out_client_handle, int32_t max_sessions, bool is_light, uintptr_t name
0x71    Result  ManageNamedPort         Handle *out_server_handle, const char *name, int32_t max_sessions
0x72    Result  ConnectToPort   Handle *out_handle, Handle port
0x73    Result  SetProcessMemoryPermission      Handle process_handle, uint64_t address, uint64_t size, MemoryPermission perm
0x74    Result  MapProcessMemory        uintptr_t dst_address, Handle process_handle, uint64_t src_address, size_t size
0x75    Result  UnmapProcessMemory      uintptr_t dst_address, Handle process_handle, uint64_t src_address, size_t size
0x76    Result  QueryProcessMemory      arch::MemoryInfo *out_memory_info, PageInfo *out_page_info, Handle process_handle, uint64_t address
0x77    Result  MapProcessCodeMemory    Handle process_handle, uint64_t dst_address, uint64_t src_address, uint64_t size
0x78    Result  UnmapProcessCodeMemory  Handle process_handle, uint64_t dst_address, uint64_t src_address, uint64_t size
0x79    Result  CreateProcess   Handle *out_handle, const arch::CreateProcessParameter *parameters, const uint32_t *caps, int32_t num_caps
0x7A    Result  StartProcess    Handle process_handle, int32_t priority, int32_t core_id, uint64_t main_thread_stack_size
0x7B    Result  TerminateProcess        Handle process_handle
0x7C    Result  GetProcessInfo  int64_t *out_info, Handle process_handle, ProcessInfoType info_type
0x7D    Result  CreateResourceLimit     Handle *out_handle
0x7E    Result  SetResourceLimitLimitValue      Handle resource_limit_handle, LimitableResource which, int64_t limit_value
0x7F    void    CallSecureMonitor       SecureMonitorArguments *args
[15.0.0+] 0x90  Result  MapInsecureMemory       uintptr_t address, size_t size
[15.0.0+] 0x91  Result  UnmapInsecureMemory     uintptr_t address, size_t size'''
lines = a.split('\n')
syscalls = [[0xffffffff,'none']]
for i in lines:
    if i.startswith('['):
        tmp = i.split()[1]
        n = i.split()[3]
    else:
        tmp = i.split()[0]
        n = i.split()[2]
    syscalls.append([int(tmp,16),n])

class sysTracer:
    callback = '''\
global tracer
asdf = ida_idd.call_stack_t()
if sysTracer.collectCallStack(asdf) == False:
    asdf = ida_idd.call_stack_t()
    if sysTracer.collectCallStack(asdf) == False:
      return False
if asdf.size() <= 0:
	return False
dup = 0x0 ^ asdf.size()
for j in range(asdf.size()-1):
	dup ^= asdf.at(j+1).callea
tracer.addCallStack(asdf,dup, ida_dbg.get_current_thread())
return False
'''
    @staticmethod
    def sysnum(function_ea):
        f = ida_funcs.get_func(function_ea)
        assert f != None
        func_start,func_end = f.start_ea,f.end_ea
        for i in range(func_end - func_start+1):
            instr = idc.GetDisasm(func_start + i)
            l = instr.split()
            if 'SVC' in l[0]:
                sys = int(l[1],16)
                return sys
        return False

    @staticmethod
    def demangleFuncName(name):
        return idc.demangle_name(name,1)
    @staticmethod
    def collectCallStack(callStack):
        return ida_dbg.collect_stack_trace(ida_dbg.get_current_thread(),callStack)
    @staticmethod
    def getBptByEA(addr):
        n = idc.get_bpt_qty()
        ret = idaapi.bpt_t()
        for i in range(n):
            assert idaapi.getn_bpt(i,ret) == True
            if ret.ea == addr:
                return ret
        return False

    def __init__(self, startSys, endSys, syscalls,target):
        self.syscalls = syscalls
        self.startSys = startSys
        self.endSys = endSys
        self.addedBpts = []
        self.savedBpts = []
        self.sysFuncs = []
        self.result = {}
        self.duplist = []
        self.target = target
        n = idc.get_bpt_qty()
        for i in range(n):
            self.savedBpts.append(idaapi.bpt_t())
            assert idaapi.getn_bpt(i,self.savedBpts[-1]) == True
    def makeSyscallFunc(self):
        # if error occurs those areas are predefined. undefine them.
        def makeFunc(function_ea):
            if not ida_funcs.get_func(function_ea):
                return ida_funcs.add_func(function_ea)
            else:
                return False
        def renameFunc(function_ea, func_name):
            return idaapi.set_name(function_ea, func_name)
        dump = ida_bytes.get_bytes(self.startSys,self.endSys-self.startSys+1)
        endIndices = [i.start() for i in re.finditer(b'\xd6', dump)]
        assert len(endIndices) > 0
        startIndices = [self.startSys] + [x+1+self.startSys for x in endIndices][:-1]
        self.sysFuncs = startIndices
        Tcnt = 0
        Fcnt = 0
        tmp = [x[0] for x in self.syscalls]
        for i in startIndices:
            if makeFunc(i):
                Tcnt += 1
            else:
                Fcnt += 1
        Rcnt = 0

        for i in startIndices:
            n = sysTracer.sysnum(i)
            if n == False:
                continue
            idx = tmp.index(n)
            if renameFunc(i,syscalls[idx][1]):
                Rcnt += 1
        return Tcnt, Fcnt,Rcnt

    def disableSavedBpts(self):
        for i in self.savedBpts:
            assert idaapi.enable_bpt(i.loc,False) == True
    def enableSavedBpts(self):
        for i in self.savedBpts:
            assert idaapi.enable_bpt(i.loc,True) == True

    def addCallStack(self,callStack,chk,thread):
        if chk in self.duplist:
            return False
        if callStack.capacity() > 0:
            sysn = sysTracer.sysnum(callStack.at(0).callea)
            if sysn not in self.result.keys():
                self.result.update({sysn : []})
            self.result[sysn].append([callStack,thread])
            self.duplist.append(chk)
        return False

    def prepTrace(self):
        assert len(self.sysFuncs) > 0
        self.disableSavedBpts()
        for i in self.sysFuncs:
            if sysTracer.sysnum(i) not in self.target:
                continue
            if idaapi.add_bpt(i, idaapi.BPT_BRK) == False:
                if sysTracer.getBptByEA(i) == False:
                    raise Exception("Setting Bpt failed")
                else:
                    if sysTracer.getBptByEA(i).enabled() == False:
                        assert idaapi.enable_bpt(sysTracer.getBptByEA(i).loc, True) == True
            self.addedBpts.append(sysTracer.getBptByEA(i))
        print(f"Added Bpt: {len(self.addedBpts)}")
        for i in self.addedBpts:
            i.elang='Python'
            i.condition = sysTracer.callback
            assert idaapi.update_bpt(i) == True

    def afterTrace(self): # clean up
        self.enableSavedBpts()
        for i in self.addedBpts:
            ida_dbg.del_bpt(i.ea)   

    def listAllCallStack(self):
        if self.result == {}:
            return False
        for i in self.result.keys():
            print(self.syscalls[i][1],f'- {i}' ,'cnt:',len(self.result[i]))

    def viewCallStack(self, sysn):
        if sysn not in self.result.keys() or self.result == {}:
            return False
        for i in range(len(self.result[sysn])):
            callStack = self.result[sysn][i]
            print(f'--- {i}th callStack Tid=({hex(callStack[1])})---')
            #idc.get_func_name(ea)
            callStack = callStack[0]
            l = callStack.size()
            for i in range(l):
                addr = callStack.at(i).callea
                funcName = idc.get_func_name(addr) if not sysTracer.demangleFuncName(idc.get_func_name(addr)) else sysTracer.demangleFuncName(idc.get_func_name(addr))
                if funcName == '':
                    funcName = hex(callStack.at(i).funcea)
                print(f"{hex(addr)} | {funcName}")
            print()

def getSysNum(s):
    global syscalls
    for i in syscalls:
        if s == i[1]:
            return i[0]
    raise Exception(f'invalid syscall "{s}"')

if __name__ == '__main__':
    # animal crossing
    startSys = 0x85B7B2B8
    endSys = 0x0000000085B7B628

    ## Splatoon2
    # startSys = 0x0008397065C
    # endSys = 0x83970C5B

    ## SuperMarioMaker2
    # startSys = 0x829CE9EC
    # endSys = 0x000829CED68

    target = [getSysNum(x) for x in ['SendSyncRequest'] ]
    tracer = sysTracer(startSys, endSys, syscalls, target)
    Tcnt, Fcnt, Rcnt = tracer.makeSyscallFunc()
    print(f"{Tcnt} funcs added")
    print(f"{Fcnt} funcs not added (already defined as a function)")
    print(f"{Rcnt} funcs renamed")
    tracer.prepTrace()
    # tracer.listAllCallStack()
    # viewCallStack(number)
    # tracer.afterTrace()