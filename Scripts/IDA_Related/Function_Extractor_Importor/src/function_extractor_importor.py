import idautils
import idaapi
import ida_funcs
import pickle

class FunctionWrapper:
    def __init__(self, func: ida_funcs.func_t):
        self.argsize = func.argsize
        self.color = func.color
        self.flags = func.flags
        self.fpd = func.fpd
        # self.frame = func.frame
        self.frregs = func.frregs
        # self.owner = func.owner
        # self.pntqty = func.pntqty
        ## self.points = func.points
        ## self.referers = func.referers
        # self.refqty = func.refqty
        # self.regargpty = func.regargqty
        ## self.regargs = func.regargs
        # self.regvarqty = func.refqty
        ## self.regvars = func.regvars
        # self.tailqty = func.tailqty
        ## self.tails = func.tails
        
        self.start_ea = func.start_ea
        self.end_ea = func.end_ea
        
    def SetToFunc(self, func: ida_funcs.func_t):
        func.argsize = self.argsize
        func.color = self.color
        func.flags = self.flags
        func.fpd = self.fpd
        # func.frame = self.frame
        func.frregs = self.frregs
        # func.owner = self.owner
        # func.pntqty = self.pntqty
        ## func.points = self.points
        ## func.referers = self.referers
        # func.refqty = self.refqty
        # func.regargqty = self.regargpty
        ## func.regargs = self.regargs
        # func.refqty = self.regvarqty
        ## func.regvars = self.regvars
        # func.tailqty = self.tailqty
        ## func.tails = self.tails
        
        func.start_ea = self.start_ea
        func.end_ea = self.end_ea

def GetImageBase():
    return idaapi.get_imagebase()

def GetFunctionList(do_ignore_sub: bool = True):
    func_list = []
    for ea in idautils.Functions():
        func = ida_funcs.get_func(ea)
        func_name = ida_funcs.get_func_name(ea)
        if(do_ignore_sub and 'sub_' in func_name or 'nullsub_' in func_name):
            continue
        func_list.append((ea, func.end_ea, func_name, FunctionWrapper(func), ))
    return func_list

def SaveFunctionListToFile(func_list, file_path):
    with open(file_path, 'wb+') as f:
        pickle.dump(func_list, f)

def ApplyFunctionList(func_list, do_ignore_sub: bool = True):
    for func_pair in func_list:
        ea, end_ea, func_name, func = func_pair[0], func_pair[1], func_pair[2], func_pair[3]
        if(do_ignore_sub and 'sub_' in func_name or 'nullsub_' in func_name):
            continue
        ida_funcs.del_func(ea)
        ida_funcs.add_func(ea, end_ea)
        try:
            func.SetToFunc(ida_funcs.get_func(ea))
        except:
            pass
        idaapi.set_name(ea, func_name)
        print(f'{ea} : {func_name}')

def LoadFunctionListFromFile(file_path):
    with open(file_path, 'rb') as f:
        return pickle.load(f)
            
def ExtractFunctionList(file_path: str, do_ignore_sub: bool = True):
    func_list = GetFunctionList(do_ignore_sub)
    SaveFunctionListToFile(func_list, file_path)

def ImportFunctionList(file_path: str, do_ignore_sub: bool = True):
    func_list = LoadFunctionListFromFile(file_path)
    ApplyFunctionList(func_list, do_ignore_sub)