# Author : Ju Chanhyoung
# Date : 2023-09-13

def is_struct_definition(s: str) -> bool:
    if(s.startswith('struct ')):
        return True
    return False

def get_struct_name(s: str) -> str:
    if(';' in s):
        return s[7:-2]
    else:
        return s[7:-1]

def get_struct_list(lines: list[str]) -> list[str]:
    struct_list: list[str] = []
    for line in lines:
        if(is_struct_definition(line)):
            struct_list.append(get_struct_name(line))
    return struct_list

def is_invalid_name(name: str) -> bool:
    l = '<> ,*()'
    for c in l:
        if(c in name):
            return True
    return False

def make_valid_name(name: str) -> str:
    name = name.replace('<', '_L_')
    name = name.replace('>', '_G_')
    name = name.replace(' ', '_S_')
    name = name.replace(',', '_C_')
    name = name.replace('*', '_P_')
    name = name.replace('(', '_OB_')
    name = name.replace(')', '_CB_')
    return name

def make_changed_name_map(name_list: list[str]) -> dict[str, str]:
    valid_name_map = dict()
    for name in name_list:
        if(not is_invalid_name(name)):
            continue
        valid_name_map[name] = make_valid_name(name)
    return valid_name_map

def replace_invalid_name(lines: list[str]):
    struct_list = get_struct_list(lines)
    valid_name_map = make_changed_name_map(struct_list)
    for i in range(len(lines)):
        for invalid_name in valid_name_map.keys():
            if(invalid_name in lines[i]):
                lines[i] = lines[i].replace(invalid_name, valid_name_map[invalid_name])
    return lines

def is_method_definition(s: str) -> bool:
    if(s.startswith('  _QWORD *') and ';' in s and '(' in s and ')' in s):
        return True
    return False

def get_method_name(s: str) -> str:
    return s[10:-2]

def get_argument_type(s: str) -> str:
    start_arguemnts = s.rfind('(')
    end_arguments = s.rfind(')')
    if(start_arguemnts != end_arguments):
        return s[start_arguemnts+1: end_arguments]
    return None
    
def get_method_list(lines: list[str]) -> list[str]:
    method_list: list[str] = []
    for line in lines:
        if(is_method_definition(line)):
            method_list.append(get_method_name(line))
    return method_list

def get_method_argument_type_list(lines: list[str]) -> list[str]:
    argument_type_list: list[str] = []
    for line in lines:
        if(is_method_definition(line)):
            argument_type_list.append(get_argument_type(get_method_name(line)))
    return argument_type_list

def is_many_arguments(argument_type: str) -> bool:
    if('_C_' in argument_type):
        return True
    return False

def is_invalid_argument_type(argument_type: str) -> bool:
    l = ['_S_', '_P_']
    if(is_many_arguments(argument_type)):
        return True
    for s in l:
        if(s in argument_type):
            return True
    return False

def recover_argument_type(argument_type: str) -> str:
    argument_type = argument_type.replace('_S_', ' ')
    argument_type = argument_type.replace('_P_', '*')
    argument_type = argument_type.replace('const_', '')
    argument_type = argument_type.replace('const', '')
    return argument_type

def make_argument_type_to_remove_list(argument_type_list: list[str]) -> list[str]:
    argument_type_to_remove_list: list[str] = []
    for argument_type in argument_type_list:
        if(is_many_arguments(argument_type)):
            argument_type_to_remove_list.append(argument_type)
    return argument_type_to_remove_list

def make_argument_type_map(argument_type_list: list[str]) -> dict[str, str]:
    argument_type_map: dict[str, str] = dict()
    for argument_type in argument_type_list:
        if(is_invalid_argument_type(argument_type) and not is_many_arguments(argument_type)):
            argument_type_map[argument_type] = recover_argument_type(argument_type)
    return argument_type_map

def remove_bad_type(lines: list[str], argument_type_to_remove_list: list[str]):
    for i in range(len(lines)):
        for type_to_remove in argument_type_to_remove_list:
            if(type_to_remove in lines[i]):
                lines[i] = lines[i].replace(type_to_remove, '')
                break
    return lines

def make_type_definition_list(argument_type_map: dict[str, str]) -> list[str]:
    type_definition_list: list[str] = []
    for argument_type in argument_type_map.keys():
        definition = f'#define {argument_type} {argument_type_map[argument_type]}\n'
        type_definition_list.append(definition)
    return type_definition_list

def insert_type_definition_list(lines: list[str], type_definition_list: list[str]) -> list[str]:
    location_to_insert = 0
    for i in range(len(lines)):
        if(lines[i].startswith('#define')):
            location_to_insert = i
        elif(location_to_insert != 0):
            break
    location_to_insert += 1
    lines = lines[:location_to_insert] + type_definition_list + lines[location_to_insert:]
    return lines
        
def make_types_valid(lines: list[str]) -> list[str]:
    argument_type_list = get_method_argument_type_list(lines)
    argument_type_to_remove_list = make_argument_type_to_remove_list(argument_type_list)
    lines = remove_bad_type(lines, argument_type_to_remove_list)
    argument_type_map = make_argument_type_map(argument_type_list)
    type_definition_list = make_type_definition_list(argument_type_map)
    lines = insert_type_definition_list(lines, type_definition_list)
    return lines

def main():
    original_file_name = 'Splatoon2_VTable_without_args.h'
    out_file_name = 'Splatoon2_VTable_without_args_processed.h'
    with open(original_file_name, 'r') as f:
        lines = f.readlines()
        lines = replace_invalid_name(lines)
        lines = replace_invalid_name(lines)
        lines = replace_invalid_name(lines)
        lines = make_types_valid(lines)
        with open(out_file_name, 'w') as f:
            for line in lines:
                f.write(line)
    
if __name__ == '__main__':
    main()