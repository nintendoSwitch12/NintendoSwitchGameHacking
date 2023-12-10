import ida_dbg
import idaapi
import ida_segment

class Mapper():

    def __init__(self,target):
        self.maps = None
        self.layout = None
        self.heap = None
        self.stack = None
        self.target = target
        self.mod = None
        self.added = []

    def setLayout(self):
        ret = []
        layouts = ida_dbg.send_dbg_command("get info")[1].split()
        heap = [int(x,16) for x in ''.join(layouts[layouts.index('Heap:')+1:layouts.index('Heap:')+4]).split('-')]
        stack = [int(x,16) for x in ''.join(layouts[layouts.index('Stack:')+1:layouts.index('Stack:')+4]).split('-')]
        for i in self.target:
            try:
                tmp = [int(x,16) for x in ''.join(layouts[layouts.index(i)-3:layouts.index(i)]).split('-')]
                ret.append(tmp)
            except:
                ret.append(-1)
                pass
        for i in range(len(ret)):
            if (ret[i] == -1):
                return False
        self.mod = ret
        self.heap = heap
        self.stack = stack
        return True
    
    def setMappings(self):
        ret = []
        maps = [x[2:] for x in ida_dbg.send_dbg_command("get mappings")[1].split('\n')][1:-1]
        for i in maps:
            prot = i[0x1c:0x1c+3]
            if prot == '   ':
                prot = '---'
            ret.append([int(i.split()[0],16),int(i.split()[2],16),prot])
        self.maps = ret
        return True

    def getContaining(self,mod):
        assert mod in self.target
        ret = []
        idx = self.target.index(mod)
        st = self.mod[idx][0]
        end = self.mod[idx][1]
        for i in self.maps:
            if st<=i[0]<=end:
                ret.append(i)
        return ret

    def go(self):
        def perm(s):
            ret = 0
            if 'r' in s:
                ret |= 0b100
            if 'w' in s:
                ret |= 0b010
            if 'x' in s:
                ret |= 0b001
            return ret
        def dup(addr):
            ret = False
            for i in range(ida_segment.get_segm_qty()):
                s = ida_segment.getnseg(i)
                st = s.start_ea
                end = s.end_ea
                if st <= addr <= end:
                    ret = True
                    break
            return ret

        self.setLayout()
        self.setMappings()
        for i in self.target:
            c = 0
            for x in self.getContaining(i):
                if 'x' in x[2]:
                    n = ".text_"+i+str(c)
                    seg_class = 'CODE'
                elif 'w' in x[2]:
                    n = ".data_"+i+str(c)
                    seg_class = 'DATA'
                else:
                    n = "."+i+str(c)
                    seg_class = 'CONST'
                if dup((x[0]+x[1])//2):
                    print(f"skipped {hex(x[0])} ~ {hex(x[1])}")
                    continue
                assert idaapi.add_segm(0, x[0], x[1]+1, n, seg_class) == True
                self.added.append(n)
                s: ida_segment.segment_t = idaapi.get_segm_by_name(n)
                s.bitness = 0x2 # 64
                # s.bitness = 0x1 # 32
                s.type = 0b11
                s.perm = perm(x[2])
                s.set_debugger_segm(False)
                s.set_loader_segm(True)
                c += 1
        print('Done')

class Analyzer():

    def __init__(self, target):
        self.target = target
        self.segms = []

    def analyze_segm(self, segm : ida_segment.segment_t ):
        st = segm.start_ea
        end = segm.end_ea
        if not segm.perm & 0b001:
            return False

        it = st
        while it<end:
            it = idaapi.find_unknown(it,1)
            idaapi.create_insn(it)
        return True
    def go(self):
        for i in self.target:
            t = ida_segment.get_segm_by_name(i) 
            assert t != None
            self.segms.append(t)
        c=  0
        for i in self.segms:
            if self.analyze_segm(i):
                c += 1
        return c



if __name__ == '__main__':
    target = ['nnrtld','TurboS.nss','multimedia','nnSdk']
    print('Once deleting predefined segment named MEMORY, this script goes well.')
    mapper = Mapper(target)
    mapper.go()
    analyzer = Analyzer(mapper.added)
    analyzer.go()