import ida_bytes
import ida_name
import idc

class Utils:
    @staticmethod
    def GetQWORD(ea) -> int:
        return ida_bytes.get_qword(ea)
    
    @staticmethod
    def GetDWORD(ea) -> int:
        return ida_bytes.get_dword(ea)
    
    @staticmethod
    def GetName(ea) -> str:
        return ida_name.get_name(ea)
    
    @staticmethod
    def SetName(ea, name: str) -> str:
        return ida_name.set_name(ea, name)
    
    @staticmethod
    def Demangle(s: str) -> str:
        return ida_name.demangle_name(s, idc.get_inf_attr(idc.INF_LONG_DN))
    
    @staticmethod
    def ParseFuncionDefinition(definition: str) -> dict:
        ret = dict()
        t: int = definition.find('(')
        if(t == -1):
            ret['name'] = definition
            ret['args'] = []
            return ret
        
        ret['name'] = definition[:t]
        definition = definition[t+1:-1]
        
        args = []
        while(len(definition) > 0):
            template_exist = definition.find('>')
            if(template_exist == -1):
                t = definition.find(', ')
            else:
                t = definition.find(', ', template_exist)
            if(t == -1):
                args.append(definition)
                break
            args.append(definition[:t])
            definition = definition[t+2:]
        ret['args'] = args
        return ret
    
    def ModifyInvalidCharInFunctionName(name: str) -> str:
        if('`' in name):
            start_name = name.find('\'') + 1
            name = name[start_name:]
        return name.replace('-', '_').replace('=', '_EQ_').replace('~', '_Tilde_')\
            .replace('<', '_LTS_').replace('>', '_GTS_').replace(' ', '_S_')\
                .replace(',', '_COMMA_').replace('*', '_STAR_')
        
class GOTReconstructor:
    def __init__(self, start, end) -> None:
        self.__start = start
        self.__end = end + 8

    def __ReconstructWithSymbol(self, ea, mangled_definition: str):
        definition = Utils.Demangle(mangled_definition)
        if(definition == None or definition == ''):
            definition = mangled_definition
        func = Utils.ParseFuncionDefinition(definition)
        func_name = Utils.ModifyInvalidCharInFunctionName(func['name'])
        func_args = func['args']
        if(Utils.SetName(ea, 'ptr_' + func_name) == False):
            redup_count: int = 0
            while(Utils.SetName(ea, 'ptr_' + func_name + f'_{redup_count}' + f'_with_{len(func_args)}_args') == False):
                redup_count += 1
    
    def __ReconstructWithoutSymbol(self, ea, definition: str):
        func_name = Utils.ModifyInvalidCharInFunctionName(definition)
        if(Utils.SetName(ea, 'ptr_' + func_name) == False):
            redup_count: int = 0
            while(Utils.SetName(ea, 'ptr_' + func_name + f'_{redup_count}') == False):
                redup_count += 1
        
    
    def Reconstruct(self, offset: int = 8):
        start = self.__start
        now = start
        end = self.__end
        
        while(now < end):
            now_name = Utils.GetName(now)
            if('qword_' in now_name or 'off_' in now_name or 'unk_' in now_name):
                if(offset == 8):
                    ea = Utils.GetQWORD(now)
                elif(offset == 4):
                    ea = Utils.GetDWORD(now)
                else:
                    assert(0)
                mangled_definition = Utils.GetName(ea)
                if(mangled_definition != None and mangled_definition != ''):
                    do_pass = False
                    do_not_follow_list = ['DynamicResolver', 'sub_', 'loc_', 'unk_', 'byte_', 'word_', 'dword_', 'qword_']
                    for token in do_not_follow_list:
                        if(token in mangled_definition):
                            do_pass = True
                    if(do_pass):
                        pass
                    else:
                        self.__ReconstructWithSymbol(now, mangled_definition)
            now += offset
            
if(__name__ == '__main__'):
    pass
    # # ACNH
    # GOTReconstructor(0x0000000084536D70,0x000000008453A798).Reconstruct()
    
    # # MK8D
    # GOTReconstructor(0x01F80674, 0x01F89AD8).Reconstruct(offset = 4)
    # GOTReconstructor(0x011154A8, 0x011160F4).Reconstruct(offset = 4)
    
    # # Splatoon 2
    # GOTReconstructor(0x837D38E8, 0x837E22E8).Reconstruct()
    # GOTReconstructor(0x82CFF620, 0x82D50000).Reconstruct()