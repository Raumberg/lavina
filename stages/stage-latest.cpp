#include "lavina.h"

namespace ir_core {
    struct IrType {
        std::string name;

    };

    struct IrParam {
        std::string name;
        IrType param_type;

    };

    struct IrLocal {
        std::string name;
        IrType local_type;

    };

    struct IrClosureSig {
        std::vector<IrType> param_types;
        IrType return_type;

    };

    struct IrCallSig {
        std::vector<IrType> param_types;
        IrType return_type;

    };

    struct IrClosureDef {
        std::string name;
        std::string helper_name;
        std::vector<IrParam> capture_params;
        std::vector<IrParam> lambda_params;
        IrClosureSig signature;

    };

    struct IrCallTarget {
        std::string kind;
        std::string name;
        IrCallSig signature;

    };

    struct IrCallPayload {
        std::string mode;
        IrCallTarget target;
        std::vector<std::string> args;

    };

    struct IrFieldPayload {
        std::string mode;
        std::string object_name;
        std::string field_name;
        std::string value_name;

    };

    struct IrIndexPayload {
        std::string mode;
        std::string object_name;
        std::string index_name;
        std::string value_name;

    };

    struct IrEnumPayload {
        std::string mode;
        std::string object_name;
        std::string variant_name;
        std::string field_name;

    };

    struct IrInst {
        std::string kind;
        std::string dest;
        IrType value_type;
        IrCallPayload call_payload;
        IrFieldPayload field_payload;
        IrIndexPayload index_payload;
        IrEnumPayload enum_payload;
        IrCallTarget call_target;
        std::string text_a;
        std::string text_b;
        std::string text_c;
        std::vector<std::string> args;

    };

    struct IrTerminator {
        std::string kind;
        std::string text_a;
        std::string text_b;
        std::string text_c;

    };

    struct IrBlock {
        std::string name;
        std::vector<IrInst> instructions;
        IrTerminator terminator;

    };

    struct IrFunction {
        std::string name;
        std::vector<IrParam> params;
        IrType return_type;
        std::vector<IrLocal> locals;
        std::vector<IrBlock> blocks;
        bool is_entry;
        bool is_extern;

    };

    struct IrModule {
        std::string name;
        std::vector<IrClosureDef> closures;
        std::vector<IrFunction> functions;

    };

} // namespace ir_core
namespace core = ir_core;
using ir_core::IrType;
using ir_core::IrParam;
using ir_core::IrLocal;
using ir_core::IrClosureSig;
using ir_core::IrCallSig;
using ir_core::IrClosureDef;
using ir_core::IrCallTarget;
using ir_core::IrCallPayload;
using ir_core::IrFieldPayload;
using ir_core::IrIndexPayload;
using ir_core::IrEnumPayload;
using ir_core::IrInst;
using ir_core::IrTerminator;
using ir_core::IrBlock;
using ir_core::IrFunction;
using ir_core::IrModule;

namespace ir_types {
    IrType ir_type(std::string name) {
        return IrType(name);
    }

    IrType ir_void_type() {
        return ir_type(std::string("void"));
    }

    IrType ir_bool_type() {
        return ir_type(std::string("bool"));
    }

    IrType ir_int_type() {
        return ir_type(std::string("int"));
    }

    IrType ir_float_type() {
        return ir_type(std::string("float"));
    }

    IrType ir_string_type() {
        return ir_type(std::string("string"));
    }

    IrType ir_bytes_type() {
        return ir_type(std::string("bytes"));
    }

    IrType ir_int8_type() {
        return ir_type(std::string("int8"));
    }

    IrType ir_int16_type() {
        return ir_type(std::string("int16"));
    }

    IrType ir_int32_type() {
        return ir_type(std::string("int32"));
    }

    IrType ir_float32_type() {
        return ir_type(std::string("float32"));
    }

    IrType ir_usize_type() {
        return ir_type(std::string("usize"));
    }

    IrType ir_auto_type() {
        return ir_type(std::string("auto"));
    }

    IrType ir_cstring_type() {
        return ir_type(std::string("*char"));
    }

    IrType ir_ptr_type(IrType inner) {
        return ir_type(((std::string("*") + (inner.name)) + std::string("")));
    }

    IrType ir_nullable_type(IrType inner) {
        return ir_type(((std::string("?") + (inner.name)) + std::string("")));
    }

    IrType ir_array_type(IrType inner) {
        return ir_type(((std::string("array[") + (inner.name)) + std::string("]")));
    }

    IrType ir_set_type(IrType inner) {
        return ir_type(((std::string("set[") + (inner.name)) + std::string("]")));
    }

    IrType ir_map_type(IrType key_type, IrType value_type) {
        return ir_type(((((std::string("map[") + (key_type.name)) + std::string(", ")) + (value_type.name)) + std::string("]")));
    }

    IrType ir_generic_closure_type() {
        return ir_type(std::string("closure"));
    }

    IrType ir_closure_type(std::string closure_name) {
        return ir_type(((std::string("closure:") + (closure_name)) + std::string("")));
    }

    bool ir_type_is_void(const IrType& t) {
        return (t.name == std::string("void"));
    }

    bool ir_type_is_bool(const IrType& t) {
        return (t.name == std::string("bool"));
    }

    bool ir_type_is_int(const IrType& t) {
        return (t.name == std::string("int"));
    }

    bool ir_type_is_float(const IrType& t) {
        return (t.name == std::string("float"));
    }

    bool ir_type_is_string(const IrType& t) {
        return (t.name == std::string("string"));
    }

    bool ir_type_is_bytes(const IrType& t) {
        return (t.name == std::string("bytes"));
    }

    bool ir_type_is_auto(const IrType& t) {
        return (t.name == std::string("auto"));
    }

    bool ir_type_has_auto(const IrType& t) {
        return lv_contains(t.name, std::string("auto"));
    }

    bool ir_type_is_array(const IrType& t) {
        return t.name.starts_with(std::string("array[")) && t.name.ends_with(std::string("]"));
    }

    bool ir_type_is_set(const IrType& t) {
        return t.name.starts_with(std::string("set[")) && t.name.ends_with(std::string("]"));
    }

    bool ir_type_is_map(const IrType& t) {
        return t.name.starts_with(std::string("map[")) && t.name.ends_with(std::string("]"));
    }

    bool ir_type_is_pair(const IrType& t) {
        return t.name.starts_with(std::string("std::pair<")) && t.name.ends_with(std::string(">"));
    }

    bool ir_type_is_ptr(const IrType& t) {
        return t.name.starts_with(std::string("*"));
    }

    bool ir_type_is_nullable(const IrType& t) {
        return t.name.starts_with(std::string("?"));
    }

    bool ir_type_is_closure(const IrType& t) {
        return t.name.starts_with(std::string("closure:"));
    }

    std::string ir_array_inner_type(const IrType& t) {
        if (ir_type_is_array(t)) {
            return t.name.substr(INT64_C(6), ((static_cast<int64_t>(t.name.size()) - INT64_C(1))) - (INT64_C(6)));
        }
        return std::string("auto");
    }

    std::string ir_set_inner_type(const IrType& t) {
        if (ir_type_is_set(t)) {
            return t.name.substr(INT64_C(4), ((static_cast<int64_t>(t.name.size()) - INT64_C(1))) - (INT64_C(4)));
        }
        return std::string("auto");
    }

    std::string ir_ptr_inner_type(const IrType& t) {
        if (ir_type_is_ptr(t)) {
            return t.name.substr(INT64_C(1), (static_cast<int64_t>(t.name.size())) - (INT64_C(1)));
        }
        return std::string("auto");
    }

    std::string ir_nullable_inner_type(const IrType& t) {
        if (ir_type_is_nullable(t)) {
            return t.name.substr(INT64_C(1), (static_cast<int64_t>(t.name.size())) - (INT64_C(1)));
        }
        return std::string("auto");
    }

    std::string ir_map_key_type_name(const IrType& t) {
        if (ir_type_is_map(t)) {
            int64_t comma_pos = lv_index_of(t.name, std::string(", "));
            if ((comma_pos >= INT64_C(0))) {
                return t.name.substr(INT64_C(4), (comma_pos) - (INT64_C(4)));
            }
        }
        return std::string("auto");
    }

    std::string ir_map_value_type_name(const IrType& t) {
        if (ir_type_is_map(t)) {
            int64_t comma_pos = lv_index_of(t.name, std::string(", "));
            if ((comma_pos >= INT64_C(0))) {
                return t.name.substr((comma_pos + INT64_C(2)), ((static_cast<int64_t>(t.name.size()) - INT64_C(1))) - ((comma_pos + INT64_C(2))));
            }
        }
        return std::string("auto");
    }

    bool ir_type_is_array_of_auto(const IrType& t) {
        return ir_type_is_array(t) && (ir_array_inner_type(t) == std::string("auto"));
    }

    bool ir_type_is_map_of_auto(const IrType& t) {
        return ir_type_is_map(t) && (ir_map_key_type_name(t) == std::string("auto")) && (ir_map_value_type_name(t) == std::string("auto"));
    }

    std::vector<std::string> ir_split_generic_args(std::string type_name) {
        std::vector<std::string> parts = {};
        std::string current = std::string("");
        int64_t depth = INT64_C(0);
        for (int64_t i = INT64_C(0); i < static_cast<int64_t>(type_name.size()); i++) {
            std::string ch = type_name.substr(i, ((i + INT64_C(1))) - (i));
            if ((ch == std::string("<"))) {
                depth = (depth + INT64_C(1));
                current = (current + ch);
            }
            else {
                if ((ch == std::string(">"))) {
                    depth = (depth - INT64_C(1));
                    current = (current + ch);
                }
                else {
                    if ((ch == std::string(",")) && (depth == INT64_C(0))) {
                        parts.push_back(lv_trim(current));
                        current = std::string("");
                    }
                    else {
                        current = (current + ch);
                    }
                }
            }
        }
        if ((current != std::string(""))) {
            parts.push_back(lv_trim(current));
        }
        return parts;
    }

    std::vector<std::string> ir_pair_type_args(const IrType& t) {
        if (ir_type_is_pair(t)) {
            std::string inner = t.name.substr(INT64_C(10), ((static_cast<int64_t>(t.name.size()) - INT64_C(1))) - (INT64_C(10)));
            return ir_split_generic_args(inner);
        }
        return {};
    }

    std::string ir_type_to_string(const IrType& t) {
        return t.name;
    }

} // namespace ir_types
namespace types = ir_types;
using ir_types::ir_type;
using ir_types::ir_void_type;
using ir_types::ir_bool_type;
using ir_types::ir_int_type;
using ir_types::ir_float_type;
using ir_types::ir_string_type;
using ir_types::ir_bytes_type;
using ir_types::ir_int8_type;
using ir_types::ir_int16_type;
using ir_types::ir_int32_type;
using ir_types::ir_float32_type;
using ir_types::ir_usize_type;
using ir_types::ir_auto_type;
using ir_types::ir_cstring_type;
using ir_types::ir_ptr_type;
using ir_types::ir_nullable_type;
using ir_types::ir_array_type;
using ir_types::ir_set_type;
using ir_types::ir_map_type;
using ir_types::ir_generic_closure_type;
using ir_types::ir_closure_type;
using ir_types::ir_type_is_void;
using ir_types::ir_type_is_bool;
using ir_types::ir_type_is_int;
using ir_types::ir_type_is_float;
using ir_types::ir_type_is_string;
using ir_types::ir_type_is_bytes;
using ir_types::ir_type_is_auto;
using ir_types::ir_type_has_auto;
using ir_types::ir_type_is_array;
using ir_types::ir_type_is_set;
using ir_types::ir_type_is_map;
using ir_types::ir_type_is_pair;
using ir_types::ir_type_is_ptr;
using ir_types::ir_type_is_nullable;
using ir_types::ir_type_is_closure;
using ir_types::ir_array_inner_type;
using ir_types::ir_set_inner_type;
using ir_types::ir_ptr_inner_type;
using ir_types::ir_nullable_inner_type;
using ir_types::ir_map_key_type_name;
using ir_types::ir_map_value_type_name;
using ir_types::ir_type_is_array_of_auto;
using ir_types::ir_type_is_map_of_auto;
using ir_types::ir_split_generic_args;
using ir_types::ir_pair_type_args;
using ir_types::ir_type_to_string;

namespace ir_calls {
    IrClosureSig ir_closure_sig(const std::vector<IrType>& param_types, IrType return_type) {
        return IrClosureSig(param_types, return_type);
    }

    IrCallSig ir_call_sig(const std::vector<IrType>& param_types, IrType return_type) {
        return IrCallSig(param_types, return_type);
    }

    IrType ir_closure_sig_return_type(const IrClosureSig& signature) {
        return signature.return_type;
    }

    std::vector<IrType> ir_closure_sig_param_types(const IrClosureSig& signature) {
        return signature.param_types;
    }

    IrType ir_call_sig_return_type(const IrCallSig& signature) {
        return signature.return_type;
    }

    std::vector<IrType> ir_call_sig_param_types(const IrCallSig& signature) {
        return signature.param_types;
    }

    IrCallTarget ir_call_target_none() {
        return IrCallTarget(std::string("none"), std::string(""), ir_call_sig({}, ir_void_type()));
    }

    IrCallTarget ir_call_target_function(std::string name, IrCallSig signature) {
        return IrCallTarget(std::string("function"), name, signature);
    }

    IrCallTarget ir_call_target_intrinsic(std::string name, IrCallSig signature) {
        return IrCallTarget(std::string("intrinsic"), name, signature);
    }

    IrCallTarget ir_call_target_closure(std::string name, IrCallSig signature) {
        return IrCallTarget(std::string("closure"), name, signature);
    }

    IrCallTarget ir_call_target_indirect(std::string name, IrCallSig signature) {
        return IrCallTarget(std::string("indirect"), name, signature);
    }

    bool ir_call_target_is_none(const IrCallTarget& target) {
        return (target.kind == std::string("none"));
    }

    bool ir_call_target_is_function(const IrCallTarget& target) {
        return (target.kind == std::string("function"));
    }

    bool ir_call_target_is_intrinsic(const IrCallTarget& target) {
        return (target.kind == std::string("intrinsic"));
    }

    bool ir_call_target_is_closure(const IrCallTarget& target) {
        return (target.kind == std::string("closure"));
    }

    bool ir_call_target_is_indirect(const IrCallTarget& target) {
        return (target.kind == std::string("indirect"));
    }

    std::string ir_call_target_name(const IrCallTarget& target) {
        return target.name;
    }

    IrCallSig ir_call_target_signature(const IrCallTarget& target) {
        return target.signature;
    }

    IrType ir_call_target_return_type(const IrCallTarget& target) {
        return ir_call_sig_return_type(target.signature);
    }

    std::vector<IrType> ir_call_target_param_types(const IrCallTarget& target) {
        return ir_call_sig_param_types(target.signature);
    }

    IrCallPayload ir_call_payload(std::string mode, IrCallTarget target, const std::vector<std::string>& args) {
        return IrCallPayload(mode, target, args);
    }

    IrCallPayload ir_call_payload_none() {
        return ir_call_payload(std::string("none"), ir_call_target_none(), {});
    }

    IrCallPayload ir_call_payload_value(IrCallTarget target, const std::vector<std::string>& args) {
        return ir_call_payload(std::string("value"), target, args);
    }

    IrCallPayload ir_call_payload_auto(IrCallTarget target, const std::vector<std::string>& args) {
        return ir_call_payload(std::string("auto"), target, args);
    }

    IrCallPayload ir_call_payload_void(IrCallTarget target, const std::vector<std::string>& args) {
        return ir_call_payload(std::string("void"), target, args);
    }

    bool ir_call_payload_is_none(const IrCallPayload& payload) {
        return (payload.mode == std::string("none"));
    }

    bool ir_call_payload_is_value(const IrCallPayload& payload) {
        return (payload.mode == std::string("value"));
    }

    bool ir_call_payload_is_auto(const IrCallPayload& payload) {
        return (payload.mode == std::string("auto"));
    }

    bool ir_call_payload_is_void(const IrCallPayload& payload) {
        return (payload.mode == std::string("void"));
    }

    std::string ir_call_payload_mode(const IrCallPayload& payload) {
        return payload.mode;
    }

    IrCallTarget ir_call_payload_target(const IrCallPayload& payload) {
        return payload.target;
    }

    std::vector<std::string> ir_call_payload_args(const IrCallPayload& payload) {
        return payload.args;
    }

    std::string ir_closure_name(const IrClosureDef& closure) {
        return closure.name;
    }

    std::string ir_closure_helper_name(const IrClosureDef& closure) {
        return closure.helper_name;
    }

    std::vector<IrParam> ir_closure_capture_params(const IrClosureDef& closure) {
        return closure.capture_params;
    }

    std::vector<IrParam> ir_closure_lambda_params(const IrClosureDef& closure) {
        return closure.lambda_params;
    }

    IrClosureSig ir_closure_signature(const IrClosureDef& closure) {
        return closure.signature;
    }

    std::string ir_function_name(const IrFunction& fn_ir) {
        return fn_ir.name;
    }

    std::vector<IrParam> ir_function_params(const IrFunction& fn_ir) {
        return fn_ir.params;
    }

    IrType ir_function_return_type(const IrFunction& fn_ir) {
        return fn_ir.return_type;
    }

    std::vector<IrLocal> ir_function_locals(const IrFunction& fn_ir) {
        return fn_ir.locals;
    }

    std::vector<IrBlock> ir_function_blocks(const IrFunction& fn_ir) {
        return fn_ir.blocks;
    }

    bool ir_function_is_entry(const IrFunction& fn_ir) {
        return fn_ir.is_entry;
    }

    std::string ir_block_name(const IrBlock& block) {
        return block.name;
    }

    std::vector<IrInst> ir_block_instructions(const IrBlock& block) {
        return block.instructions;
    }

    IrTerminator ir_block_terminator(const IrBlock& block) {
        return block.terminator;
    }

    std::string ir_module_name(const IrModule& module_ir) {
        return module_ir.name;
    }

    std::vector<IrClosureDef> ir_module_closures(const IrModule& module_ir) {
        return module_ir.closures;
    }

    std::vector<IrFunction> ir_module_functions(const IrModule& module_ir) {
        return module_ir.functions;
    }

} // namespace ir_calls
namespace calls = ir_calls;
using ir_calls::ir_closure_sig;
using ir_calls::ir_call_sig;
using ir_calls::ir_closure_sig_return_type;
using ir_calls::ir_closure_sig_param_types;
using ir_calls::ir_call_sig_return_type;
using ir_calls::ir_call_sig_param_types;
using ir_calls::ir_call_target_none;
using ir_calls::ir_call_target_function;
using ir_calls::ir_call_target_intrinsic;
using ir_calls::ir_call_target_closure;
using ir_calls::ir_call_target_indirect;
using ir_calls::ir_call_target_is_none;
using ir_calls::ir_call_target_is_function;
using ir_calls::ir_call_target_is_intrinsic;
using ir_calls::ir_call_target_is_closure;
using ir_calls::ir_call_target_is_indirect;
using ir_calls::ir_call_target_name;
using ir_calls::ir_call_target_signature;
using ir_calls::ir_call_target_return_type;
using ir_calls::ir_call_target_param_types;
using ir_calls::ir_call_payload;
using ir_calls::ir_call_payload_none;
using ir_calls::ir_call_payload_value;
using ir_calls::ir_call_payload_auto;
using ir_calls::ir_call_payload_void;
using ir_calls::ir_call_payload_is_none;
using ir_calls::ir_call_payload_is_value;
using ir_calls::ir_call_payload_is_auto;
using ir_calls::ir_call_payload_is_void;
using ir_calls::ir_call_payload_mode;
using ir_calls::ir_call_payload_target;
using ir_calls::ir_call_payload_args;
using ir_calls::ir_closure_name;
using ir_calls::ir_closure_helper_name;
using ir_calls::ir_closure_capture_params;
using ir_calls::ir_closure_lambda_params;
using ir_calls::ir_closure_signature;
using ir_calls::ir_function_name;
using ir_calls::ir_function_params;
using ir_calls::ir_function_return_type;
using ir_calls::ir_function_locals;
using ir_calls::ir_function_blocks;
using ir_calls::ir_function_is_entry;
using ir_calls::ir_block_name;
using ir_calls::ir_block_instructions;
using ir_calls::ir_block_terminator;
using ir_calls::ir_module_name;
using ir_calls::ir_module_closures;
using ir_calls::ir_module_functions;

namespace ir_payloads {
    IrFieldPayload ir_field_payload(std::string mode, std::string object_name, std::string field_name, std::string value_name) {
        return IrFieldPayload(mode, object_name, field_name, value_name);
    }

    IrFieldPayload ir_field_payload_none() {
        return ir_field_payload(std::string("none"), std::string(""), std::string(""), std::string(""));
    }

    IrFieldPayload ir_field_payload_read(std::string object_name, std::string field_name) {
        return ir_field_payload(std::string("read"), object_name, field_name, std::string(""));
    }

    IrFieldPayload ir_field_payload_write(std::string object_name, std::string field_name, std::string value_name) {
        return ir_field_payload(std::string("write"), object_name, field_name, value_name);
    }

    bool ir_field_payload_is_none(const IrFieldPayload& payload) {
        return (payload.mode == std::string("none"));
    }

    bool ir_field_payload_is_read(const IrFieldPayload& payload) {
        return (payload.mode == std::string("read"));
    }

    bool ir_field_payload_is_write(const IrFieldPayload& payload) {
        return (payload.mode == std::string("write"));
    }

    std::string ir_field_payload_object(const IrFieldPayload& payload) {
        return payload.object_name;
    }

    std::string ir_field_payload_name(const IrFieldPayload& payload) {
        return payload.field_name;
    }

    std::string ir_field_payload_value(const IrFieldPayload& payload) {
        return payload.value_name;
    }

    IrIndexPayload ir_index_payload(std::string mode, std::string object_name, std::string index_name, std::string value_name) {
        return IrIndexPayload(mode, object_name, index_name, value_name);
    }

    IrIndexPayload ir_index_payload_none() {
        return ir_index_payload(std::string("none"), std::string(""), std::string(""), std::string(""));
    }

    IrIndexPayload ir_index_payload_read(std::string object_name, std::string index_name) {
        return ir_index_payload(std::string("read"), object_name, index_name, std::string(""));
    }

    IrIndexPayload ir_index_payload_write(std::string object_name, std::string index_name, std::string value_name) {
        return ir_index_payload(std::string("write"), object_name, index_name, value_name);
    }

    bool ir_index_payload_is_none(const IrIndexPayload& payload) {
        return (payload.mode == std::string("none"));
    }

    bool ir_index_payload_is_read(const IrIndexPayload& payload) {
        return (payload.mode == std::string("read"));
    }

    bool ir_index_payload_is_write(const IrIndexPayload& payload) {
        return (payload.mode == std::string("write"));
    }

    std::string ir_index_payload_object(const IrIndexPayload& payload) {
        return payload.object_name;
    }

    std::string ir_index_payload_index(const IrIndexPayload& payload) {
        return payload.index_name;
    }

    std::string ir_index_payload_value(const IrIndexPayload& payload) {
        return payload.value_name;
    }

    IrEnumPayload ir_enum_payload(std::string mode, std::string object_name, std::string variant_name, std::string field_name) {
        return IrEnumPayload(mode, object_name, variant_name, field_name);
    }

    IrEnumPayload ir_enum_payload_none() {
        return ir_enum_payload(std::string("none"), std::string(""), std::string(""), std::string(""));
    }

    IrEnumPayload ir_enum_payload_check(std::string object_name, std::string variant_name) {
        return ir_enum_payload(std::string("check"), object_name, variant_name, std::string(""));
    }

    IrEnumPayload ir_enum_payload_read(std::string object_name, std::string variant_name, std::string field_name) {
        return ir_enum_payload(std::string("read"), object_name, variant_name, field_name);
    }

    bool ir_enum_payload_is_none(const IrEnumPayload& payload) {
        return (payload.mode == std::string("none"));
    }

    bool ir_enum_payload_is_check(const IrEnumPayload& payload) {
        return (payload.mode == std::string("check"));
    }

    bool ir_enum_payload_is_read(const IrEnumPayload& payload) {
        return (payload.mode == std::string("read"));
    }

    std::string ir_enum_payload_object(const IrEnumPayload& payload) {
        return payload.object_name;
    }

    std::string ir_enum_payload_variant(const IrEnumPayload& payload) {
        return payload.variant_name;
    }

    std::string ir_enum_payload_field(const IrEnumPayload& payload) {
        return payload.field_name;
    }

} // namespace ir_payloads
namespace payloads = ir_payloads;
using ir_payloads::ir_field_payload;
using ir_payloads::ir_field_payload_none;
using ir_payloads::ir_field_payload_read;
using ir_payloads::ir_field_payload_write;
using ir_payloads::ir_field_payload_is_none;
using ir_payloads::ir_field_payload_is_read;
using ir_payloads::ir_field_payload_is_write;
using ir_payloads::ir_field_payload_object;
using ir_payloads::ir_field_payload_name;
using ir_payloads::ir_field_payload_value;
using ir_payloads::ir_index_payload;
using ir_payloads::ir_index_payload_none;
using ir_payloads::ir_index_payload_read;
using ir_payloads::ir_index_payload_write;
using ir_payloads::ir_index_payload_is_none;
using ir_payloads::ir_index_payload_is_read;
using ir_payloads::ir_index_payload_is_write;
using ir_payloads::ir_index_payload_object;
using ir_payloads::ir_index_payload_index;
using ir_payloads::ir_index_payload_value;
using ir_payloads::ir_enum_payload;
using ir_payloads::ir_enum_payload_none;
using ir_payloads::ir_enum_payload_check;
using ir_payloads::ir_enum_payload_read;
using ir_payloads::ir_enum_payload_is_none;
using ir_payloads::ir_enum_payload_is_check;
using ir_payloads::ir_enum_payload_is_read;
using ir_payloads::ir_enum_payload_object;
using ir_payloads::ir_enum_payload_variant;
using ir_payloads::ir_enum_payload_field;

namespace ir_insts {
    IrInst ir_const(std::string dest, IrType value_type, std::string literal_kind, std::string literal_value) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("const"), dest, value_type, ir_call_payload_none(), ir_field_payload_none(), ir_index_payload_none(), ir_enum_payload_none(), ir_call_target_none(), literal_kind, literal_value, std::string(""), empty_args);
    }

    IrInst ir_const_int(std::string dest, IrType value_type, std::string literal_value) {
        return ir_const(dest, value_type, std::string("int"), literal_value);
    }

    IrInst ir_vector_literal(std::string dest, IrType value_type, const std::vector<std::string>& elements) {
        return IrInst(std::string("vector_literal"), dest, value_type, ir_call_payload_none(), ir_field_payload_none(), ir_index_payload_none(), ir_enum_payload_none(), ir_call_target_none(), std::string(""), std::string(""), std::string(""), elements);
    }

    IrInst ir_map_literal(std::string dest, IrType value_type, const std::vector<std::string>& items) {
        return IrInst(std::string("map_literal"), dest, value_type, ir_call_payload_none(), ir_field_payload_none(), ir_index_payload_none(), ir_enum_payload_none(), ir_call_target_none(), std::string(""), std::string(""), std::string(""), items);
    }

    IrInst ir_move(std::string dest, IrType value_type, std::string source) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("move"), dest, value_type, ir_call_payload_none(), ir_field_payload_none(), ir_index_payload_none(), ir_enum_payload_none(), ir_call_target_none(), source, std::string(""), std::string(""), empty_args);
    }

    IrInst ir_unary(std::string dest, IrType value_type, std::string op, std::string operand) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("unary"), dest, value_type, ir_call_payload_none(), ir_field_payload_none(), ir_index_payload_none(), ir_enum_payload_none(), ir_call_target_none(), op, operand, std::string(""), empty_args);
    }

    IrInst ir_binary(std::string dest, IrType value_type, std::string op, std::string left, std::string right) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("binary"), dest, value_type, ir_call_payload_none(), ir_field_payload_none(), ir_index_payload_none(), ir_enum_payload_none(), ir_call_target_none(), op, left, right, empty_args);
    }

    IrInst ir_binary_add(std::string dest, IrType value_type, std::string left, std::string right) {
        return ir_binary(dest, value_type, std::string("+"), left, right);
    }

    IrInst ir_binary_lt(std::string dest, std::string left, std::string right) {
        return ir_binary(dest, ir_bool_type(), std::string("<"), left, right);
    }

    IrInst ir_call(std::string dest, IrType value_type, IrCallTarget call_target, const std::vector<std::string>& args) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("call"), dest, value_type, ir_call_payload_value(call_target, args), ir_field_payload_none(), ir_index_payload_none(), ir_enum_payload_none(), ir_call_target_none(), std::string(""), std::string(""), std::string(""), empty_args);
    }

    IrInst ir_call_auto(std::string dest, IrCallTarget call_target, const std::vector<std::string>& args) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("call_auto"), dest, ir_auto_type(), ir_call_payload_auto(call_target, args), ir_field_payload_none(), ir_index_payload_none(), ir_enum_payload_none(), ir_call_target_none(), std::string(""), std::string(""), std::string(""), empty_args);
    }

    IrInst ir_call_void(IrCallTarget call_target, const std::vector<std::string>& args) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("call_void"), std::string(""), ir_void_type(), ir_call_payload_void(call_target, args), ir_field_payload_none(), ir_index_payload_none(), ir_enum_payload_none(), ir_call_target_none(), std::string(""), std::string(""), std::string(""), empty_args);
    }

    bool ir_inst_is_const(const IrInst& inst) {
        return (inst.kind == std::string("const"));
    }

    bool ir_inst_is_vector_literal(const IrInst& inst) {
        return (inst.kind == std::string("vector_literal"));
    }

    bool ir_inst_is_map_literal(const IrInst& inst) {
        return (inst.kind == std::string("map_literal"));
    }

    bool ir_inst_is_move(const IrInst& inst) {
        return (inst.kind == std::string("move"));
    }

    bool ir_inst_is_unary(const IrInst& inst) {
        return (inst.kind == std::string("unary"));
    }

    bool ir_inst_is_binary(const IrInst& inst) {
        return (inst.kind == std::string("binary"));
    }

    bool ir_inst_is_call(const IrInst& inst) {
        if ((!ir_call_payload_is_none(inst.call_payload))) {
            return ir_call_payload_is_value(inst.call_payload);
        }
        return (inst.kind == std::string("call"));
    }

    bool ir_inst_is_call_auto(const IrInst& inst) {
        if ((!ir_call_payload_is_none(inst.call_payload))) {
            return ir_call_payload_is_auto(inst.call_payload);
        }
        return (inst.kind == std::string("call_auto"));
    }

    bool ir_inst_is_call_void(const IrInst& inst) {
        if ((!ir_call_payload_is_none(inst.call_payload))) {
            return ir_call_payload_is_void(inst.call_payload);
        }
        return (inst.kind == std::string("call_void"));
    }

    bool ir_inst_is_call_like(const IrInst& inst) {
        return ir_inst_is_call(inst) || ir_inst_is_call_auto(inst) || ir_inst_is_call_void(inst);
    }

    bool ir_inst_is_get_field(const IrInst& inst) {
        return (inst.kind == std::string("get_field"));
    }

    bool ir_inst_is_set_field(const IrInst& inst) {
        return (inst.kind == std::string("set_field"));
    }

    bool ir_inst_is_index(const IrInst& inst) {
        return (inst.kind == std::string("index"));
    }

    bool ir_inst_is_index_set(const IrInst& inst) {
        return (inst.kind == std::string("index_set"));
    }

    bool ir_inst_is_convert(const IrInst& inst) {
        return (inst.kind == std::string("convert"));
    }

    bool ir_inst_is_bind_auto(const IrInst& inst) {
        return (inst.kind == std::string("bind_auto"));
    }

    bool ir_inst_is_make_closure(const IrInst& inst) {
        return (inst.kind == std::string("make_closure"));
    }

    bool ir_inst_is_enum_is(const IrInst& inst) {
        if ((!ir_enum_payload_is_none(inst.enum_payload))) {
            return ir_enum_payload_is_check(inst.enum_payload);
        }
        return (inst.kind == std::string("enum_is"));
    }

    bool ir_inst_is_enum_get(const IrInst& inst) {
        if ((!ir_enum_payload_is_none(inst.enum_payload))) {
            return ir_enum_payload_is_read(inst.enum_payload);
        }
        return (inst.kind == std::string("enum_get"));
    }

    bool ir_inst_is_field_op(const IrInst& inst) {
        return ir_inst_is_get_field(inst) || ir_inst_is_set_field(inst);
    }

    bool ir_inst_is_field_read(const IrInst& inst) {
        return ir_inst_is_get_field(inst);
    }

    bool ir_inst_is_field_write(const IrInst& inst) {
        return ir_inst_is_set_field(inst);
    }

    bool ir_inst_is_index_op(const IrInst& inst) {
        return ir_inst_is_index(inst) || ir_inst_is_index_set(inst);
    }

    bool ir_inst_is_index_read(const IrInst& inst) {
        return ir_inst_is_index(inst);
    }

    bool ir_inst_is_index_write(const IrInst& inst) {
        return ir_inst_is_index_set(inst);
    }

    bool ir_inst_is_enum_op(const IrInst& inst) {
        return ir_inst_is_enum_is(inst) || ir_inst_is_enum_get(inst);
    }

    bool ir_inst_is_enum_check(const IrInst& inst) {
        return ir_inst_is_enum_is(inst);
    }

    bool ir_inst_is_enum_read(const IrInst& inst) {
        return ir_inst_is_enum_get(inst);
    }

    std::string ir_inst_dest(const IrInst& inst) {
        return inst.dest;
    }

    IrType ir_inst_value_type(const IrInst& inst) {
        return inst.value_type;
    }

    IrCallTarget ir_inst_call_target(const IrInst& inst) {
        if ((!ir_call_payload_is_none(inst.call_payload))) {
            return ir_call_payload_target(inst.call_payload);
        }
        return inst.call_target;
    }

    std::string ir_inst_call_mode(const IrInst& inst) {
        if ((!ir_call_payload_is_none(inst.call_payload))) {
            return ir_call_payload_mode(inst.call_payload);
        }
        if (ir_inst_is_call(inst)) {
            return std::string("value");
        }
        else {
            if (ir_inst_is_call_auto(inst)) {
                return std::string("auto");
            }
            else {
                if (ir_inst_is_call_void(inst)) {
                    return std::string("void");
                }
            }
        }
        return std::string("none");
    }

    std::string ir_inst_call_target_name(const IrInst& inst) {
        return ir_call_target_name(ir_inst_call_target(inst));
    }

    IrType ir_inst_call_target_return_type(const IrInst& inst) {
        return ir_call_target_return_type(ir_inst_call_target(inst));
    }

    std::vector<IrType> ir_inst_call_target_param_types(const IrInst& inst) {
        return ir_call_target_param_types(ir_inst_call_target(inst));
    }

    std::vector<std::string> ir_inst_args(const IrInst& inst) {
        if ((!ir_call_payload_is_none(inst.call_payload))) {
            return ir_call_payload_args(inst.call_payload);
        }
        return inst.args;
    }

    std::string ir_inst_literal_kind(const IrInst& inst) {
        return inst.text_a;
    }

    std::string ir_inst_literal_value(const IrInst& inst) {
        return inst.text_b;
    }

    std::string ir_inst_source_name(const IrInst& inst) {
        return inst.text_a;
    }

    std::string ir_inst_unary_op(const IrInst& inst) {
        return inst.text_a;
    }

    std::string ir_inst_unary_operand(const IrInst& inst) {
        return inst.text_b;
    }

    std::string ir_inst_binary_op(const IrInst& inst) {
        return inst.text_a;
    }

    std::string ir_inst_binary_left(const IrInst& inst) {
        return inst.text_b;
    }

    std::string ir_inst_binary_right(const IrInst& inst) {
        return inst.text_c;
    }

    std::string ir_inst_field_object(const IrInst& inst) {
        if ((!ir_field_payload_is_none(inst.field_payload))) {
            return ir_field_payload_object(inst.field_payload);
        }
        return inst.text_a;
    }

    std::string ir_inst_field_name(const IrInst& inst) {
        if ((!ir_field_payload_is_none(inst.field_payload))) {
            return ir_field_payload_name(inst.field_payload);
        }
        return inst.text_b;
    }

    std::string ir_inst_field_value(const IrInst& inst) {
        if ((!ir_field_payload_is_none(inst.field_payload))) {
            return ir_field_payload_value(inst.field_payload);
        }
        return inst.text_c;
    }

    std::string ir_inst_index_object(const IrInst& inst) {
        if ((!ir_index_payload_is_none(inst.index_payload))) {
            return ir_index_payload_object(inst.index_payload);
        }
        return inst.text_a;
    }

    std::string ir_inst_index_value(const IrInst& inst) {
        if ((!ir_index_payload_is_none(inst.index_payload))) {
            return ir_index_payload_index(inst.index_payload);
        }
        return inst.text_b;
    }

    std::string ir_inst_index_set_value(const IrInst& inst) {
        if ((!ir_index_payload_is_none(inst.index_payload))) {
            return ir_index_payload_value(inst.index_payload);
        }
        return inst.text_c;
    }

    std::string ir_inst_bind_expr(const IrInst& inst) {
        return inst.text_a;
    }

    std::string ir_inst_closure_name(const IrInst& inst) {
        return inst.text_a;
    }

    std::string ir_inst_enum_object(const IrInst& inst) {
        if ((!ir_enum_payload_is_none(inst.enum_payload))) {
            return ir_enum_payload_object(inst.enum_payload);
        }
        return inst.text_a;
    }

    std::string ir_inst_enum_variant(const IrInst& inst) {
        if ((!ir_enum_payload_is_none(inst.enum_payload))) {
            return ir_enum_payload_variant(inst.enum_payload);
        }
        return inst.text_b;
    }

    std::string ir_inst_enum_field(const IrInst& inst) {
        if ((!ir_enum_payload_is_none(inst.enum_payload))) {
            return ir_enum_payload_field(inst.enum_payload);
        }
        return inst.text_c;
    }

    IrInst ir_get_field(std::string dest, IrType value_type, std::string object_name, std::string field_name) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("get_field"), dest, value_type, ir_call_payload_none(), ir_field_payload_read(object_name, field_name), ir_index_payload_none(), ir_enum_payload_none(), ir_call_target_none(), std::string(""), std::string(""), std::string(""), empty_args);
    }

    IrInst ir_set_field(std::string object_name, std::string field_name, std::string value_name) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("set_field"), std::string(""), ir_void_type(), ir_call_payload_none(), ir_field_payload_write(object_name, field_name, value_name), ir_index_payload_none(), ir_enum_payload_none(), ir_call_target_none(), std::string(""), std::string(""), std::string(""), empty_args);
    }

    IrInst ir_index(std::string dest, IrType value_type, std::string object_name, std::string index_name) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("index"), dest, value_type, ir_call_payload_none(), ir_field_payload_none(), ir_index_payload_read(object_name, index_name), ir_enum_payload_none(), ir_call_target_none(), std::string(""), std::string(""), std::string(""), empty_args);
    }

    IrInst ir_index_set(std::string object_name, std::string index_name, std::string value_name) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("index_set"), std::string(""), ir_void_type(), ir_call_payload_none(), ir_field_payload_none(), ir_index_payload_write(object_name, index_name, value_name), ir_enum_payload_none(), ir_call_target_none(), std::string(""), std::string(""), std::string(""), empty_args);
    }

    IrInst ir_convert(std::string dest, IrType value_type, std::string source) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("convert"), dest, value_type, ir_call_payload_none(), ir_field_payload_none(), ir_index_payload_none(), ir_enum_payload_none(), ir_call_target_none(), source, std::string(""), std::string(""), empty_args);
    }

    IrInst ir_bind_auto(std::string dest, std::string expr_text) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("bind_auto"), dest, ir_auto_type(), ir_call_payload_none(), ir_field_payload_none(), ir_index_payload_none(), ir_enum_payload_none(), ir_call_target_none(), expr_text, std::string(""), std::string(""), empty_args);
    }

    IrInst ir_make_closure(std::string dest, IrType value_type, std::string closure_name, const std::vector<std::string>& captures) {
        return IrInst(std::string("make_closure"), dest, value_type, ir_call_payload_none(), ir_field_payload_none(), ir_index_payload_none(), ir_enum_payload_none(), ir_call_target_none(), closure_name, std::string(""), std::string(""), captures);
    }

    IrInst ir_enum_is(std::string dest, std::string object_name, std::string variant_name) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("enum_is"), dest, ir_bool_type(), ir_call_payload_none(), ir_field_payload_none(), ir_index_payload_none(), ir_enum_payload_check(object_name, variant_name), ir_call_target_none(), std::string(""), std::string(""), std::string(""), empty_args);
    }

    IrInst ir_enum_get(std::string dest, IrType value_type, std::string object_name, std::string variant_name, std::string field_name) {
        std::vector<std::string> empty_args = {};
        return IrInst(std::string("enum_get"), dest, value_type, ir_call_payload_none(), ir_field_payload_none(), ir_index_payload_none(), ir_enum_payload_read(object_name, variant_name, field_name), ir_call_target_none(), std::string(""), std::string(""), std::string(""), empty_args);
    }

} // namespace ir_insts
namespace insts = ir_insts;
using ir_insts::ir_const;
using ir_insts::ir_const_int;
using ir_insts::ir_vector_literal;
using ir_insts::ir_map_literal;
using ir_insts::ir_move;
using ir_insts::ir_unary;
using ir_insts::ir_binary;
using ir_insts::ir_binary_add;
using ir_insts::ir_binary_lt;
using ir_insts::ir_call;
using ir_insts::ir_call_auto;
using ir_insts::ir_call_void;
using ir_insts::ir_inst_is_const;
using ir_insts::ir_inst_is_vector_literal;
using ir_insts::ir_inst_is_map_literal;
using ir_insts::ir_inst_is_move;
using ir_insts::ir_inst_is_unary;
using ir_insts::ir_inst_is_binary;
using ir_insts::ir_inst_is_call;
using ir_insts::ir_inst_is_call_auto;
using ir_insts::ir_inst_is_call_void;
using ir_insts::ir_inst_is_call_like;
using ir_insts::ir_inst_is_get_field;
using ir_insts::ir_inst_is_set_field;
using ir_insts::ir_inst_is_index;
using ir_insts::ir_inst_is_index_set;
using ir_insts::ir_inst_is_convert;
using ir_insts::ir_inst_is_bind_auto;
using ir_insts::ir_inst_is_make_closure;
using ir_insts::ir_inst_is_enum_is;
using ir_insts::ir_inst_is_enum_get;
using ir_insts::ir_inst_is_field_op;
using ir_insts::ir_inst_is_field_read;
using ir_insts::ir_inst_is_field_write;
using ir_insts::ir_inst_is_index_op;
using ir_insts::ir_inst_is_index_read;
using ir_insts::ir_inst_is_index_write;
using ir_insts::ir_inst_is_enum_op;
using ir_insts::ir_inst_is_enum_check;
using ir_insts::ir_inst_is_enum_read;
using ir_insts::ir_inst_dest;
using ir_insts::ir_inst_value_type;
using ir_insts::ir_inst_call_target;
using ir_insts::ir_inst_call_mode;
using ir_insts::ir_inst_call_target_name;
using ir_insts::ir_inst_call_target_return_type;
using ir_insts::ir_inst_call_target_param_types;
using ir_insts::ir_inst_args;
using ir_insts::ir_inst_literal_kind;
using ir_insts::ir_inst_literal_value;
using ir_insts::ir_inst_source_name;
using ir_insts::ir_inst_unary_op;
using ir_insts::ir_inst_unary_operand;
using ir_insts::ir_inst_binary_op;
using ir_insts::ir_inst_binary_left;
using ir_insts::ir_inst_binary_right;
using ir_insts::ir_inst_field_object;
using ir_insts::ir_inst_field_name;
using ir_insts::ir_inst_field_value;
using ir_insts::ir_inst_index_object;
using ir_insts::ir_inst_index_value;
using ir_insts::ir_inst_index_set_value;
using ir_insts::ir_inst_bind_expr;
using ir_insts::ir_inst_closure_name;
using ir_insts::ir_inst_enum_object;
using ir_insts::ir_inst_enum_variant;
using ir_insts::ir_inst_enum_field;
using ir_insts::ir_get_field;
using ir_insts::ir_set_field;
using ir_insts::ir_index;
using ir_insts::ir_index_set;
using ir_insts::ir_convert;
using ir_insts::ir_bind_auto;
using ir_insts::ir_make_closure;
using ir_insts::ir_enum_is;
using ir_insts::ir_enum_get;

namespace ir_terms {
    IrTerminator ir_term_none() {
        return IrTerminator(std::string("none"), std::string(""), std::string(""), std::string(""));
    }

    IrTerminator ir_term_jump(std::string target) {
        return IrTerminator(std::string("jump"), target, std::string(""), std::string(""));
    }

    IrTerminator ir_term_branch(std::string condition, std::string then_block, std::string else_block) {
        return IrTerminator(std::string("branch"), condition, then_block, else_block);
    }

    IrTerminator ir_term_return_void() {
        return IrTerminator(std::string("return_void"), std::string(""), std::string(""), std::string(""));
    }

    IrTerminator ir_term_return_value(std::string value) {
        return IrTerminator(std::string("return_value"), value, std::string(""), std::string(""));
    }

    IrTerminator ir_term_unreachable() {
        return IrTerminator(std::string("unreachable"), std::string(""), std::string(""), std::string(""));
    }

    bool ir_term_is_none(const IrTerminator& term) {
        return (term.kind == std::string("none"));
    }

    bool ir_term_is_jump(const IrTerminator& term) {
        return (term.kind == std::string("jump"));
    }

    bool ir_term_is_branch(const IrTerminator& term) {
        return (term.kind == std::string("branch"));
    }

    bool ir_term_is_return_void(const IrTerminator& term) {
        return (term.kind == std::string("return_void"));
    }

    bool ir_term_is_return_value(const IrTerminator& term) {
        return (term.kind == std::string("return_value"));
    }

    bool ir_term_is_unreachable(const IrTerminator& term) {
        return (term.kind == std::string("unreachable"));
    }

    std::string ir_term_condition(const IrTerminator& term) {
        return term.text_a;
    }

    std::string ir_term_then_target(const IrTerminator& term) {
        return term.text_b;
    }

    std::string ir_term_else_target(const IrTerminator& term) {
        return term.text_c;
    }

    std::string ir_term_jump_target(const IrTerminator& term) {
        return term.text_a;
    }

    std::string ir_term_return_value_name(const IrTerminator& term) {
        return term.text_a;
    }

} // namespace ir_terms
namespace terms = ir_terms;
using ir_terms::ir_term_none;
using ir_terms::ir_term_jump;
using ir_terms::ir_term_branch;
using ir_terms::ir_term_return_void;
using ir_terms::ir_term_return_value;
using ir_terms::ir_term_unreachable;
using ir_terms::ir_term_is_none;
using ir_terms::ir_term_is_jump;
using ir_terms::ir_term_is_branch;
using ir_terms::ir_term_is_return_void;
using ir_terms::ir_term_is_return_value;
using ir_terms::ir_term_is_unreachable;
using ir_terms::ir_term_condition;
using ir_terms::ir_term_then_target;
using ir_terms::ir_term_else_target;
using ir_terms::ir_term_jump_target;
using ir_terms::ir_term_return_value_name;

const std::string TK_LEFT_PAREN = std::string("LeftParen");
const std::string TK_RIGHT_PAREN = std::string("RightParen");
const std::string TK_LEFT_BRACKET = std::string("LeftBracket");
const std::string TK_RIGHT_BRACKET = std::string("RightBracket");
const std::string TK_LEFT_BRACE = std::string("LeftBrace");
const std::string TK_RIGHT_BRACE = std::string("RightBrace");
const std::string TK_COMMA = std::string("Comma");
const std::string TK_DOT = std::string("Dot");
const std::string TK_DOT_DOT = std::string("DotDot");
const std::string TK_MINUS = std::string("Minus");
const std::string TK_PLUS = std::string("Plus");
const std::string TK_SEMICOLON = std::string("Semicolon");
const std::string TK_SLASH = std::string("Slash");
const std::string TK_STAR = std::string("Star");
const std::string TK_COLON = std::string("Colon");
const std::string TK_HASH = std::string("Hash");
const std::string TK_QUESTION = std::string("Question");
const std::string TK_PERCENT = std::string("Percent");
const std::string TK_PLUS_EQUAL = std::string("PlusEqual");
const std::string TK_MINUS_EQUAL = std::string("MinusEqual");
const std::string TK_STAR_EQUAL = std::string("StarEqual");
const std::string TK_BANG = std::string("Bang");
const std::string TK_BANG_EQUAL = std::string("BangEqual");
const std::string TK_EQUAL = std::string("Equal");
const std::string TK_EQUAL_EQUAL = std::string("EqualEqual");
const std::string TK_GREATER = std::string("Greater");
const std::string TK_GREATER_EQUAL = std::string("GreaterEqual");
const std::string TK_LESS = std::string("Less");
const std::string TK_LESS_EQUAL = std::string("LessEqual");
const std::string TK_ARROW = std::string("Arrow");
const std::string TK_FAT_ARROW = std::string("FatArrow");
const std::string TK_DOUBLE_COLON = std::string("DoubleColon");
const std::string TK_IDENTIFIER = std::string("Identifier");
const std::string TK_STRING = std::string("String");
const std::string TK_INT = std::string("Int");
const std::string TK_FLOAT = std::string("Float");
const std::string TK_AND = std::string("And");
const std::string TK_AUTO = std::string("Auto");
const std::string TK_BOOL = std::string("Bool");
const std::string TK_COMPTIME = std::string("Comptime");
const std::string TK_COMPTIME_STRICT = std::string("ComptimeStrict");
const std::string TK_ELSE = std::string("Else");
const std::string TK_ELIF = std::string("Elif");
const std::string TK_FALSE = std::string("False");
const std::string TK_FLOAT_TYPE = std::string("FloatType");
const std::string TK_FN = std::string("Fn");
const std::string TK_IF = std::string("If");
const std::string TK_INLINE = std::string("Inline");
const std::string TK_INT_TYPE = std::string("IntType");
const std::string TK_NULL = std::string("Null");
const std::string TK_OR = std::string("Or");
const std::string TK_RETURN = std::string("Return");
const std::string TK_STRING_TYPE = std::string("StringType");
const std::string TK_TRUE = std::string("True");
const std::string TK_VOID = std::string("Void");
const std::string TK_WHILE = std::string("While");
const std::string TK_FOR = std::string("For");
const std::string TK_IN = std::string("In");
const std::string TK_VECTOR = std::string("Vector");
const std::string TK_HASHMAP = std::string("HashMap");
const std::string TK_HASHSET = std::string("HashSet");
const std::string TK_IMPORT = std::string("Import");
const std::string TK_AS = std::string("As");
const std::string TK_NAMESPACE = std::string("Namespace");
const std::string TK_PUBLIC = std::string("Public");
const std::string TK_PRIVATE = std::string("Private");
const std::string TK_STATIC = std::string("Static");
const std::string TK_CLASS = std::string("Class");
const std::string TK_STRUCT = std::string("Struct");
const std::string TK_ENUM = std::string("Enum");
const std::string TK_EXTEND = std::string("Extend");
const std::string TK_THIS = std::string("This");
const std::string TK_TRY = std::string("Try");
const std::string TK_CATCH = std::string("Catch");
const std::string TK_THROW = std::string("Throw");
const std::string TK_CONST = std::string("Const");
const std::string TK_LET = std::string("Let");
const std::string TK_MATCH = std::string("Match");
const std::string TK_BREAK = std::string("Break");
const std::string TK_CONTINUE = std::string("Continue");
const std::string TK_NOT = std::string("Not");
const std::string TK_REF = std::string("Ref");
const std::string TK_REF_MUT = std::string("RefMut");
const std::string TK_OWN = std::string("Own");
const std::string TK_PASS = std::string("Pass");
const std::string TK_INT8 = std::string("Int8");
const std::string TK_INT16 = std::string("Int16");
const std::string TK_INT32 = std::string("Int32");
const std::string TK_INT64 = std::string("Int64");
const std::string TK_FLOAT32 = std::string("Float32");
const std::string TK_FLOAT64 = std::string("Float64");
const std::string TK_USIZE = std::string("USize");
const std::string TK_CSTRING = std::string("CString");
const std::string TK_PTR = std::string("Ptr");
const std::string TK_BYTES = std::string("Bytes");
const std::string TK_CPP = std::string("Cpp");
const std::string TK_AMPERSAND = std::string("Ampersand");
const std::string TK_EXTERN = std::string("Extern");
const std::string TK_LINK = std::string("Link");
const std::string TK_OPERATOR = std::string("Operator");
const std::string TK_INDENT = std::string("Indent");
const std::string TK_DEDENT = std::string("Dedent");
const std::string TK_NEWLINE = std::string("Newline");
const std::string TK_EOF = std::string("Eof");
struct Token {
    std::string token_type;
    std::string lexeme;
    int64_t line;
    int64_t col;

    Token(std::string token_type, std::string lexeme, int64_t line, int64_t col)
        : token_type(token_type), lexeme(lexeme), line(line), col(col) {
    }

    std::string to_string() const {
        return ((((((((std::string("") + (this->token_type)) + std::string("(")) + (this->lexeme)) + std::string(") at ")) + (this->line)) + std::string(":")) + (this->col)) + std::string(""));
    }

};

void print(const Token& _v) { std::cout << _v.to_string() << std::endl; }
std::string operator+(const std::string& _s, const Token& _v) { return _s + _v.to_string(); }
std::string operator+(const Token& _v, const std::string& _s) { return _v.to_string() + _s; }

std::string lookup_keyword(const std::string& w) {
    if ((w == std::string("and"))) {
        return TK_AND;
    }
    else {
        if ((w == std::string("auto"))) {
            return TK_AUTO;
        }
        else {
            if ((w == std::string("bool"))) {
                return TK_BOOL;
            }
            else {
                if ((w == std::string("comptime"))) {
                    return TK_COMPTIME;
                }
                else {
                    if ((w == std::string("else"))) {
                        return TK_ELSE;
                    }
                    else {
                        if ((w == std::string("elif"))) {
                            return TK_ELIF;
                        }
                        else {
                            if ((w == std::string("false"))) {
                                return TK_FALSE;
                            }
                            else {
                                if ((w == std::string("float"))) {
                                    return TK_FLOAT_TYPE;
                                }
                                else {
                                    if ((w == std::string("fn"))) {
                                        return TK_FN;
                                    }
                                    else {
                                        if ((w == std::string("if"))) {
                                            return TK_IF;
                                        }
                                        else {
                                            if ((w == std::string("inline"))) {
                                                return TK_INLINE;
                                            }
                                            else {
                                                if ((w == std::string("int"))) {
                                                    return TK_INT_TYPE;
                                                }
                                                else {
                                                    if ((w == std::string("null"))) {
                                                        return TK_NULL;
                                                    }
                                                    else {
                                                        if ((w == std::string("or"))) {
                                                            return TK_OR;
                                                        }
                                                        else {
                                                            if ((w == std::string("return"))) {
                                                                return TK_RETURN;
                                                            }
                                                            else {
                                                                if ((w == std::string("string"))) {
                                                                    return TK_STRING_TYPE;
                                                                }
                                                                else {
                                                                    if ((w == std::string("true"))) {
                                                                        return TK_TRUE;
                                                                    }
                                                                    else {
                                                                        if ((w == std::string("void"))) {
                                                                            return TK_VOID;
                                                                        }
                                                                        else {
                                                                            if ((w == std::string("while"))) {
                                                                                return TK_WHILE;
                                                                            }
                                                                            else {
                                                                                if ((w == std::string("for"))) {
                                                                                    return TK_FOR;
                                                                                }
                                                                                else {
                                                                                    if ((w == std::string("in"))) {
                                                                                        return TK_IN;
                                                                                    }
                                                                                    else {
                                                                                        if ((w == std::string("vector"))) {
                                                                                            return TK_VECTOR;
                                                                                        }
                                                                                        else {
                                                                                            if ((w == std::string("hashmap"))) {
                                                                                                return TK_HASHMAP;
                                                                                            }
                                                                                            else {
                                                                                                if ((w == std::string("hashset"))) {
                                                                                                    return TK_HASHSET;
                                                                                                }
                                                                                                else {
                                                                                                    if ((w == std::string("import"))) {
                                                                                                        return TK_IMPORT;
                                                                                                    }
                                                                                                    else {
                                                                                                        if ((w == std::string("as"))) {
                                                                                                            return TK_AS;
                                                                                                        }
                                                                                                        else {
                                                                                                            if ((w == std::string("namespace"))) {
                                                                                                                return TK_NAMESPACE;
                                                                                                            }
                                                                                                            else {
                                                                                                                if ((w == std::string("public"))) {
                                                                                                                    return TK_PUBLIC;
                                                                                                                }
                                                                                                                else {
                                                                                                                    if ((w == std::string("private"))) {
                                                                                                                        return TK_PRIVATE;
                                                                                                                    }
                                                                                                                    else {
                                                                                                                        if ((w == std::string("static"))) {
                                                                                                                            return TK_STATIC;
                                                                                                                        }
                                                                                                                        else {
                                                                                                                            if ((w == std::string("class"))) {
                                                                                                                                return TK_CLASS;
                                                                                                                            }
                                                                                                                            else {
                                                                                                                                if ((w == std::string("struct"))) {
                                                                                                                                    return TK_STRUCT;
                                                                                                                                }
                                                                                                                                else {
                                                                                                                                    if ((w == std::string("enum"))) {
                                                                                                                                        return TK_ENUM;
                                                                                                                                    }
                                                                                                                                    else {
                                                                                                                                        if ((w == std::string("this"))) {
                                                                                                                                            return TK_THIS;
                                                                                                                                        }
                                                                                                                                        else {
                                                                                                                                            if ((w == std::string("try"))) {
                                                                                                                                                return TK_TRY;
                                                                                                                                            }
                                                                                                                                            else {
                                                                                                                                                if ((w == std::string("catch"))) {
                                                                                                                                                    return TK_CATCH;
                                                                                                                                                }
                                                                                                                                                else {
                                                                                                                                                    if ((w == std::string("throw"))) {
                                                                                                                                                        return TK_THROW;
                                                                                                                                                    }
                                                                                                                                                    else {
                                                                                                                                                        if ((w == std::string("const"))) {
                                                                                                                                                            return TK_CONST;
                                                                                                                                                        }
                                                                                                                                                        else {
                                                                                                                                                            if ((w == std::string("let"))) {
                                                                                                                                                                return TK_LET;
                                                                                                                                                            }
                                                                                                                                                            else {
                                                                                                                                                                if ((w == std::string("match"))) {
                                                                                                                                                                    return TK_MATCH;
                                                                                                                                                                }
                                                                                                                                                                else {
                                                                                                                                                                    if ((w == std::string("break"))) {
                                                                                                                                                                        return TK_BREAK;
                                                                                                                                                                    }
                                                                                                                                                                    else {
                                                                                                                                                                        if ((w == std::string("continue"))) {
                                                                                                                                                                            return TK_CONTINUE;
                                                                                                                                                                        }
                                                                                                                                                                        else {
                                                                                                                                                                            if ((w == std::string("not"))) {
                                                                                                                                                                                return TK_NOT;
                                                                                                                                                                            }
                                                                                                                                                                            else {
                                                                                                                                                                                if ((w == std::string("ref"))) {
                                                                                                                                                                                    return TK_REF;
                                                                                                                                                                                }
                                                                                                                                                                                else {
                                                                                                                                                                                    if ((w == std::string("own"))) {
                                                                                                                                                                                        return TK_OWN;
                                                                                                                                                                                    }
                                                                                                                                                                                    else {
                                                                                                                                                                                        if ((w == std::string("pass"))) {
                                                                                                                                                                                            return TK_PASS;
                                                                                                                                                                                        }
                                                                                                                                                                                        else {
                                                                                                                                                                                            if ((w == std::string("int8"))) {
                                                                                                                                                                                                return TK_INT8;
                                                                                                                                                                                            }
                                                                                                                                                                                            else {
                                                                                                                                                                                                if ((w == std::string("int16"))) {
                                                                                                                                                                                                    return TK_INT16;
                                                                                                                                                                                                }
                                                                                                                                                                                                else {
                                                                                                                                                                                                    if ((w == std::string("int32"))) {
                                                                                                                                                                                                        return TK_INT32;
                                                                                                                                                                                                    }
                                                                                                                                                                                                    else {
                                                                                                                                                                                                        if ((w == std::string("int64"))) {
                                                                                                                                                                                                            return TK_INT64;
                                                                                                                                                                                                        }
                                                                                                                                                                                                        else {
                                                                                                                                                                                                            if ((w == std::string("float32"))) {
                                                                                                                                                                                                                return TK_FLOAT32;
                                                                                                                                                                                                            }
                                                                                                                                                                                                            else {
                                                                                                                                                                                                                if ((w == std::string("float64"))) {
                                                                                                                                                                                                                    return TK_FLOAT64;
                                                                                                                                                                                                                }
                                                                                                                                                                                                                else {
                                                                                                                                                                                                                    if ((w == std::string("usize"))) {
                                                                                                                                                                                                                        return TK_USIZE;
                                                                                                                                                                                                                    }
                                                                                                                                                                                                                    else {
                                                                                                                                                                                                                        if ((w == std::string("cstring"))) {
                                                                                                                                                                                                                            return TK_CSTRING;
                                                                                                                                                                                                                        }
                                                                                                                                                                                                                        else {
                                                                                                                                                                                                                            if ((w == std::string("ptr"))) {
                                                                                                                                                                                                                                return TK_PTR;
                                                                                                                                                                                                                            }
                                                                                                                                                                                                                            else {
                                                                                                                                                                                                                                if ((w == std::string("bytes"))) {
                                                                                                                                                                                                                                    return TK_BYTES;
                                                                                                                                                                                                                                }
                                                                                                                                                                                                                                else {
                                                                                                                                                                                                                                    if ((w == std::string("cpp"))) {
                                                                                                                                                                                                                                        return TK_CPP;
                                                                                                                                                                                                                                    }
                                                                                                                                                                                                                                    else {
                                                                                                                                                                                                                                        if ((w == std::string("extern"))) {
                                                                                                                                                                                                                                            return TK_EXTERN;
                                                                                                                                                                                                                                        }
                                                                                                                                                                                                                                        else {
                                                                                                                                                                                                                                            if ((w == std::string("link"))) {
                                                                                                                                                                                                                                                return TK_LINK;
                                                                                                                                                                                                                                            }
                                                                                                                                                                                                                                            else {
                                                                                                                                                                                                                                                if ((w == std::string("operator"))) {
                                                                                                                                                                                                                                                    return TK_OPERATOR;
                                                                                                                                                                                                                                                }
                                                                                                                                                                                                                                                else {
                                                                                                                                                                                                                                                    if ((w == std::string("extend"))) {
                                                                                                                                                                                                                                                        return TK_EXTEND;
                                                                                                                                                                                                                                                    }
                                                                                                                                                                                                                                                }
                                                                                                                                                                                                                                            }
                                                                                                                                                                                                                                        }
                                                                                                                                                                                                                                    }
                                                                                                                                                                                                                                }
                                                                                                                                                                                                                            }
                                                                                                                                                                                                                        }
                                                                                                                                                                                                                    }
                                                                                                                                                                                                                }
                                                                                                                                                                                                            }
                                                                                                                                                                                                        }
                                                                                                                                                                                                    }
                                                                                                                                                                                                }
                                                                                                                                                                                            }
                                                                                                                                                                                        }
                                                                                                                                                                                    }
                                                                                                                                                                                }
                                                                                                                                                                            }
                                                                                                                                                                        }
                                                                                                                                                                    }
                                                                                                                                                                }
                                                                                                                                                            }
                                                                                                                                                        }
                                                                                                                                                    }
                                                                                                                                                }
                                                                                                                                            }
                                                                                                                                        }
                                                                                                                                    }
                                                                                                                                }
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return std::string("");
}

bool is_alpha(auto c) {
    if ((c >= std::string("a")) && (c <= std::string("z"))) {
        return true;
    }
    else {
        if ((c >= std::string("A")) && (c <= std::string("Z"))) {
            return true;
        }
        else {
            if ((c == std::string("_"))) {
                return true;
            }
        }
    }
    return false;
}

bool is_digit(auto c) {
    if ((c >= std::string("0")) && (c <= std::string("9"))) {
        return true;
    }
    return false;
}

bool is_hex_digit(auto c) {
    if (is_digit(c)) {
        return true;
    }
    if ((c >= std::string("a")) && (c <= std::string("f"))) {
        return true;
    }
    if ((c >= std::string("A")) && (c <= std::string("F"))) {
        return true;
    }
    return false;
}

bool is_alnum(auto c) {
    if (is_alpha(c)) {
        return true;
    }
    if (is_digit(c)) {
        return true;
    }
    return false;
}

struct Scanner {
    std::string source;
    std::vector<Token> tokens;
    std::vector<std::string> errors;
    int64_t start;
    int64_t current;
    int64_t line;
    int64_t column;
    int64_t start_line;
    int64_t start_column;
    std::vector<int64_t> indent_stack;
    bool at_line_start;
    bool in_string_interp;
    int64_t interp_brace_depth;
    int64_t bracket_depth;

    Scanner(std::string source)
        : source(source) {
        this->tokens = {};
        this->errors = {};
        this->start = INT64_C(0);
        this->current = INT64_C(0);
        this->line = INT64_C(1);
        this->column = INT64_C(1);
        this->start_line = INT64_C(1);
        this->start_column = INT64_C(1);
        this->indent_stack = std::vector{INT64_C(0)};
        this->at_line_start = true;
        this->in_string_interp = false;
        this->interp_brace_depth = INT64_C(0);
        this->bracket_depth = INT64_C(0);
    }

    bool is_at_end() {
        return (this->current >= static_cast<int64_t>(this->source.size()));
    }

    auto peek() {
        if ((*this).is_at_end()) {
            return std::string("");
        }
        return std::string(1, this->source[this->current]);
    }

    auto peek_next() {
        if (((this->current + INT64_C(1)) >= static_cast<int64_t>(this->source.size()))) {
            return std::string("");
        }
        return std::string(1, this->source[(this->current + INT64_C(1))]);
    }

    auto advance() {
        auto c = std::string(1, this->source[this->current]);
        this->current = (this->current + INT64_C(1));
        this->column = (this->column + INT64_C(1));
        return c;
    }

    bool match_char(std::string expected) {
        if ((*this).is_at_end()) {
            return false;
        }
        if ((std::string(1, this->source[this->current]) != expected)) {
            return false;
        }
        this->current = (this->current + INT64_C(1));
        this->column = (this->column + INT64_C(1));
        return true;
    }

    void add_token(std::string tk_type, std::string lexeme) {
        int64_t sl = this->start_line;
        int64_t sc = this->start_column;
        auto tok = Token(tk_type, lexeme, sl, sc);
        this->tokens.push_back(tok);
    }

    void add_simple_token(std::string tk_type) {
        auto lexeme = this->source.substr(this->start, (this->current) - (this->start));
        (*this).add_token(tk_type, lexeme);
    }

    void add_error(std::string message) {
        auto err = ((((((std::string("Error at line ") + (this->line)) + std::string(":")) + (this->column)) + std::string(": ")) + (message)) + std::string(""));
        this->errors.push_back(err);
    }

    void handle_indentation() {
        if ((this->bracket_depth > INT64_C(0))) {
            while ((!(*this).is_at_end()) && (((*this).peek() == std::string(" ")) || ((*this).peek() == std::string("\t")))) {
                (*this).advance();
            }
            this->at_line_start = false;
            return;
        }
        int64_t indent = INT64_C(0);
        while ((!(*this).is_at_end()) && (((*this).peek() == std::string(" ")) || ((*this).peek() == std::string("\t")))) {
            auto c = (*this).advance();
            if ((c == std::string(" "))) {
                indent = (indent + INT64_C(1));
            }
            else {
                indent = (indent + INT64_C(4));
            }
        }
        if ((*this).is_at_end() || ((*this).peek() == std::string("\n"))) {
            if (((*this).peek() == std::string("\n"))) {
                this->at_line_start = true;
            }
            return;
        }
        auto current_indent = this->indent_stack[(static_cast<int64_t>(this->indent_stack.size()) - INT64_C(1))];
        if ((indent > current_indent)) {
            this->indent_stack.push_back(indent);
            (*this).add_token(TK_INDENT, std::string(""));
        }
        else {
            if ((indent < current_indent)) {
                while ((indent < this->indent_stack[(static_cast<int64_t>(this->indent_stack.size()) - INT64_C(1))])) {
                    lv_pop(this->indent_stack);
                    (*this).add_token(TK_DEDENT, std::string(""));
                }
            }
        }
        this->at_line_start = false;
    }

    void scan_string() {
        int64_t str_start = this->current;
        while ((!(*this).is_at_end()) && ((*this).peek() != std::string("\""))) {
            if (((*this).peek() == std::string("\\"))) {
                (*this).advance();
                (*this).advance();
            }
            else {
                if (((*this).peek() == std::string("$")) && ((*this).peek_next() == std::string("{"))) {
                    auto value = this->source.substr(str_start, (this->current) - (str_start));
                    (*this).add_token(TK_STRING, value);
                    (*this).add_token(TK_PLUS, std::string("+"));
                    (*this).add_token(TK_LEFT_PAREN, std::string("("));
                    (*this).advance();
                    (*this).advance();
                    this->in_string_interp = true;
                    this->interp_brace_depth = INT64_C(1);
                    return;
                }
                else {
                    if (((*this).peek() == std::string("\n"))) {
                        this->line = (this->line + INT64_C(1));
                        this->column = INT64_C(1);
                    }
                    (*this).advance();
                }
            }
        }
        if ((*this).is_at_end()) {
            (*this).add_error(std::string("Unterminated string"));
            return;
        }
        auto value = this->source.substr(str_start, (this->current) - (str_start));
        (*this).advance();
        (*this).add_token(TK_STRING, value);
    }

    void scan_number() {
        if ((std::string(1, this->source[this->start]) == std::string("0")) && (((*this).peek() == std::string("x")) || ((*this).peek() == std::string("X")))) {
            (*this).advance();
            while (is_hex_digit((*this).peek())) {
                (*this).advance();
            }
            (*this).add_simple_token(TK_INT);
            return;
        }
        while (is_digit((*this).peek())) {
            (*this).advance();
        }
        if (((*this).peek() == std::string(".")) && is_digit((*this).peek_next())) {
            (*this).advance();
            while (is_digit((*this).peek())) {
                (*this).advance();
            }
            (*this).add_simple_token(TK_FLOAT);
        }
        else {
            (*this).add_simple_token(TK_INT);
        }
    }

    void scan_identifier() {
        while (is_alnum((*this).peek())) {
            (*this).advance();
        }
        auto text = this->source.substr(this->start, (this->current) - (this->start));
        auto kw = lookup_keyword(text);
        if ((kw == TK_COMPTIME) && ((*this).peek() == std::string("!"))) {
            (*this).advance();
            (*this).add_token(TK_COMPTIME_STRICT, std::string("comptime!"));
        }
        else {
            if ((kw == TK_REF) && ((*this).peek() == std::string("!"))) {
                (*this).advance();
                (*this).add_token(TK_REF_MUT, std::string("ref!"));
            }
            else {
                if ((kw == TK_CPP)) {
                    int64_t save = this->current;
                    int64_t save_col = this->column;
                    while ((!(*this).is_at_end()) && (((*this).peek() == std::string(" ")) || ((*this).peek() == std::string("\t")))) {
                        (*this).advance();
                    }
                    if ((!(*this).is_at_end()) && ((*this).peek() == std::string("{"))) {
                        this->current = save;
                        this->column = save_col;
                        (*this).scan_cpp_block();
                    }
                    else {
                        this->current = save;
                        this->column = save_col;
                        (*this).add_token(TK_IDENTIFIER, text);
                    }
                }
                else {
                    if ((kw != std::string(""))) {
                        (*this).add_token(kw, text);
                    }
                    else {
                        (*this).add_token(TK_IDENTIFIER, text);
                    }
                }
            }
        }
    }

    void scan_cpp_block() {
        while ((!(*this).is_at_end()) && (((*this).peek() == std::string(" ")) || ((*this).peek() == std::string("\t")))) {
            (*this).advance();
        }
        if ((*this).is_at_end() || ((*this).peek() != std::string("{"))) {
            (*this).add_error(std::string("Expect '{' after 'cpp'"));
            return;
        }
        (*this).advance();
        int64_t depth = INT64_C(1);
        int64_t content_start = this->current;
        while ((!(*this).is_at_end()) && (depth > INT64_C(0))) {
            auto c = (*this).advance();
            if ((c == std::string("{"))) {
                depth = (depth + INT64_C(1));
            }
            else {
                if ((c == std::string("}"))) {
                    depth = (depth - INT64_C(1));
                }
                else {
                    if ((c == std::string("\n"))) {
                        this->line = (this->line + INT64_C(1));
                        this->column = INT64_C(1);
                    }
                }
            }
        }
        if ((depth != INT64_C(0))) {
            (*this).add_error(std::string("Unterminated cpp block"));
            return;
        }
        auto content = this->source.substr(content_start, ((this->current - INT64_C(1))) - (content_start));
        (*this).add_token(TK_CPP, content);
    }

    void scan_token() {
        if (this->at_line_start) {
            (*this).handle_indentation();
            this->start = this->current;
            this->start_line = this->line;
            this->start_column = this->column;
        }
        if ((*this).is_at_end()) {
            return;
        }
        auto c = (*this).advance();
        if ((c == std::string("("))) {
            (*this).add_simple_token(TK_LEFT_PAREN);
        }
        else {
            if ((c == std::string(")"))) {
                (*this).add_simple_token(TK_RIGHT_PAREN);
            }
            else {
                if ((c == std::string("["))) {
                    this->bracket_depth = (this->bracket_depth + INT64_C(1));
                    (*this).add_simple_token(TK_LEFT_BRACKET);
                }
                else {
                    if ((c == std::string("]"))) {
                        if ((this->bracket_depth > INT64_C(0))) {
                            this->bracket_depth = (this->bracket_depth - INT64_C(1));
                        }
                        (*this).add_simple_token(TK_RIGHT_BRACKET);
                    }
                    else {
                        if ((c == std::string("{"))) {
                            this->bracket_depth = (this->bracket_depth + INT64_C(1));
                            if (this->in_string_interp) {
                                this->interp_brace_depth = (this->interp_brace_depth + INT64_C(1));
                            }
                            (*this).add_simple_token(TK_LEFT_BRACE);
                        }
                        else {
                            if ((c == std::string("}"))) {
                                if (this->in_string_interp) {
                                    this->interp_brace_depth = (this->interp_brace_depth - INT64_C(1));
                                    if ((this->interp_brace_depth == INT64_C(0))) {
                                        this->in_string_interp = false;
                                        (*this).add_token(TK_RIGHT_PAREN, std::string(")"));
                                        (*this).add_token(TK_PLUS, std::string("+"));
                                        (*this).scan_string();
                                        return;
                                    }
                                }
                                if ((this->bracket_depth > INT64_C(0))) {
                                    this->bracket_depth = (this->bracket_depth - INT64_C(1));
                                }
                                (*this).add_simple_token(TK_RIGHT_BRACE);
                            }
                            else {
                                if ((c == std::string(","))) {
                                    (*this).add_simple_token(TK_COMMA);
                                }
                                else {
                                    if ((c == std::string("."))) {
                                        if ((*this).match_char(std::string("."))) {
                                            (*this).add_simple_token(TK_DOT_DOT);
                                        }
                                        else {
                                            (*this).add_simple_token(TK_DOT);
                                        }
                                    }
                                    else {
                                        if ((c == std::string("?"))) {
                                            (*this).add_simple_token(TK_QUESTION);
                                        }
                                        else {
                                            if ((c == std::string("+"))) {
                                                if ((*this).match_char(std::string("="))) {
                                                    (*this).add_simple_token(TK_PLUS_EQUAL);
                                                }
                                                else {
                                                    (*this).add_simple_token(TK_PLUS);
                                                }
                                            }
                                            else {
                                                if ((c == std::string(";"))) {
                                                    (*this).add_simple_token(TK_SEMICOLON);
                                                }
                                                else {
                                                    if ((c == std::string("*"))) {
                                                        if ((*this).match_char(std::string("="))) {
                                                            (*this).add_simple_token(TK_STAR_EQUAL);
                                                        }
                                                        else {
                                                            (*this).add_simple_token(TK_STAR);
                                                        }
                                                    }
                                                    else {
                                                        if ((c == std::string("%"))) {
                                                            (*this).add_simple_token(TK_PERCENT);
                                                        }
                                                        else {
                                                            if ((c == std::string("&"))) {
                                                                if ((*this).match_char(std::string("&"))) {
                                                                    (*this).add_simple_token(TK_AND);
                                                                }
                                                                else {
                                                                    (*this).add_simple_token(TK_AMPERSAND);
                                                                }
                                                            }
                                                            else {
                                                                if ((c == std::string("|"))) {
                                                                    if ((*this).match_char(std::string("|"))) {
                                                                        (*this).add_simple_token(TK_OR);
                                                                    }
                                                                    else {
                                                                        (*this).add_error(std::string("Unexpected character: |"));
                                                                    }
                                                                }
                                                                else {
                                                                    if ((c == std::string("-"))) {
                                                                        if ((*this).match_char(std::string(">"))) {
                                                                            (*this).add_simple_token(TK_ARROW);
                                                                        }
                                                                        else {
                                                                            if ((*this).match_char(std::string("="))) {
                                                                                (*this).add_simple_token(TK_MINUS_EQUAL);
                                                                            }
                                                                            else {
                                                                                (*this).add_simple_token(TK_MINUS);
                                                                            }
                                                                        }
                                                                    }
                                                                    else {
                                                                        if ((c == std::string(":"))) {
                                                                            if ((*this).match_char(std::string(":"))) {
                                                                                (*this).add_simple_token(TK_DOUBLE_COLON);
                                                                            }
                                                                            else {
                                                                                (*this).add_simple_token(TK_COLON);
                                                                            }
                                                                        }
                                                                        else {
                                                                            if ((c == std::string("#"))) {
                                                                                (*this).add_simple_token(TK_HASH);
                                                                            }
                                                                            else {
                                                                                if ((c == std::string("!"))) {
                                                                                    if ((*this).match_char(std::string("="))) {
                                                                                        (*this).add_simple_token(TK_BANG_EQUAL);
                                                                                    }
                                                                                    else {
                                                                                        (*this).add_simple_token(TK_BANG);
                                                                                    }
                                                                                }
                                                                                else {
                                                                                    if ((c == std::string("="))) {
                                                                                        if ((*this).match_char(std::string("="))) {
                                                                                            (*this).add_simple_token(TK_EQUAL_EQUAL);
                                                                                        }
                                                                                        else {
                                                                                            if ((*this).match_char(std::string(">"))) {
                                                                                                (*this).add_simple_token(TK_FAT_ARROW);
                                                                                            }
                                                                                            else {
                                                                                                (*this).add_simple_token(TK_EQUAL);
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                    else {
                                                                                        if ((c == std::string("<"))) {
                                                                                            if ((*this).match_char(std::string("="))) {
                                                                                                (*this).add_simple_token(TK_LESS_EQUAL);
                                                                                            }
                                                                                            else {
                                                                                                (*this).add_simple_token(TK_LESS);
                                                                                            }
                                                                                        }
                                                                                        else {
                                                                                            if ((c == std::string(">"))) {
                                                                                                if ((*this).match_char(std::string("="))) {
                                                                                                    (*this).add_simple_token(TK_GREATER_EQUAL);
                                                                                                }
                                                                                                else {
                                                                                                    (*this).add_simple_token(TK_GREATER);
                                                                                                }
                                                                                            }
                                                                                            else {
                                                                                                if ((c == std::string("/"))) {
                                                                                                    if ((*this).match_char(std::string("/"))) {
                                                                                                        while (((*this).peek() != std::string("\n")) && (!(*this).is_at_end())) {
                                                                                                            (*this).advance();
                                                                                                        }
                                                                                                    }
                                                                                                    else {
                                                                                                        (*this).add_simple_token(TK_SLASH);
                                                                                                    }
                                                                                                }
                                                                                                else {
                                                                                                    if ((c == std::string(" ")) || (c == std::string("\r")) || (c == std::string("\t"))) {
                                                                                                        /* pass */
                                                                                                    }
                                                                                                    else {
                                                                                                        if ((c == std::string("\n"))) {
                                                                                                            if ((this->bracket_depth == INT64_C(0))) {
                                                                                                                (*this).add_token(TK_NEWLINE, std::string("\n"));
                                                                                                            }
                                                                                                            this->line = (this->line + INT64_C(1));
                                                                                                            this->column = INT64_C(1);
                                                                                                            if ((this->bracket_depth == INT64_C(0))) {
                                                                                                                this->at_line_start = true;
                                                                                                            }
                                                                                                        }
                                                                                                        else {
                                                                                                            if ((c == std::string("\""))) {
                                                                                                                (*this).scan_string();
                                                                                                            }
                                                                                                            else {
                                                                                                                if (is_digit(c)) {
                                                                                                                    (*this).scan_number();
                                                                                                                }
                                                                                                                else {
                                                                                                                    if (is_alpha(c)) {
                                                                                                                        (*this).scan_identifier();
                                                                                                                    }
                                                                                                                    else {
                                                                                                                        (*this).add_error(((std::string("Unexpected character: ") + (c)) + std::string("")));
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void scan_tokens() {
        while ((!(*this).is_at_end())) {
            this->start = this->current;
            this->start_line = this->line;
            this->start_column = this->column;
            (*this).scan_token();
        }
        while ((static_cast<int64_t>(this->indent_stack.size()) > INT64_C(1))) {
            lv_pop(this->indent_stack);
            (*this).add_token(TK_DEDENT, std::string(""));
        }
        (*this).add_token(TK_EOF, std::string(""));
    }

};

struct TypeNode;
struct TypeNode {
    struct None {};
    struct Int {};
    struct Float {};
    struct Str {};
    struct Bool {};
    struct Void {};
    struct Auto {};
    struct NullType {};
    struct Custom { std::string name; std::vector<TypeNode> type_args; };
    struct Array { std::shared_ptr<TypeNode> inner; };
    struct HashSet { std::shared_ptr<TypeNode> inner; };
    struct HashMap { std::shared_ptr<TypeNode> key_type; std::shared_ptr<TypeNode> value_type; };
    struct Nullable { std::shared_ptr<TypeNode> inner; };
    struct Int8 {};
    struct Int16 {};
    struct Int32 {};
    struct Float32 {};
    struct USize {};
    struct CString {};
    struct Ptr { std::shared_ptr<TypeNode> inner; };
    struct Bytes {};

    std::string _tag;
    std::variant<TypeNode::None, TypeNode::Int, TypeNode::Float, TypeNode::Str, TypeNode::Bool, TypeNode::Void, TypeNode::Auto, TypeNode::NullType, TypeNode::Custom, TypeNode::Array, TypeNode::HashSet, TypeNode::HashMap, TypeNode::Nullable, TypeNode::Int8, TypeNode::Int16, TypeNode::Int32, TypeNode::Float32, TypeNode::USize, TypeNode::CString, TypeNode::Ptr, TypeNode::Bytes> _data;

    static TypeNode make_None() { return {"None", None{}}; }
    static TypeNode make_Int() { return {"Int", Int{}}; }
    static TypeNode make_Float() { return {"Float", Float{}}; }
    static TypeNode make_Str() { return {"Str", Str{}}; }
    static TypeNode make_Bool() { return {"Bool", Bool{}}; }
    static TypeNode make_Void() { return {"Void", Void{}}; }
    static TypeNode make_Auto() { return {"Auto", Auto{}}; }
    static TypeNode make_NullType() { return {"NullType", NullType{}}; }
    static TypeNode make_Custom(std::string name, std::vector<TypeNode> type_args) { return {"Custom", Custom{name, type_args}}; }
    static TypeNode make_Array(TypeNode inner) { return {"Array", Array{std::make_shared<TypeNode>(std::move(inner))}}; }
    static TypeNode make_HashSet(TypeNode inner) { return {"HashSet", HashSet{std::make_shared<TypeNode>(std::move(inner))}}; }
    static TypeNode make_HashMap(TypeNode key_type, TypeNode value_type) { return {"HashMap", HashMap{std::make_shared<TypeNode>(std::move(key_type)), std::make_shared<TypeNode>(std::move(value_type))}}; }
    static TypeNode make_Nullable(TypeNode inner) { return {"Nullable", Nullable{std::make_shared<TypeNode>(std::move(inner))}}; }
    static TypeNode make_Int8() { return {"Int8", Int8{}}; }
    static TypeNode make_Int16() { return {"Int16", Int16{}}; }
    static TypeNode make_Int32() { return {"Int32", Int32{}}; }
    static TypeNode make_Float32() { return {"Float32", Float32{}}; }
    static TypeNode make_USize() { return {"USize", USize{}}; }
    static TypeNode make_CString() { return {"CString", CString{}}; }
    static TypeNode make_Ptr(TypeNode inner) { return {"Ptr", Ptr{std::make_shared<TypeNode>(std::move(inner))}}; }
    static TypeNode make_Bytes() { return {"Bytes", Bytes{}}; }

    std::string operator[](const std::string& key) const {
        if (key == "_tag") return _tag;
        return "";
    }
};

void print(const TypeNode& _v) { std::cout << "TypeNode(" << _v._tag << ")" << std::endl; }
std::string operator+(const std::string& _s, const TypeNode& _v) { return _s + _v._tag; }
std::string operator+(const TypeNode& _v, const std::string& _s) { return _v._tag + _s; }

struct Param {
    Token name;
    TypeNode param_type;
    bool is_ref;
    bool is_mut;

};

struct EnumVariantNode {
    Token name;
    std::vector<TypeNode> types;
    std::vector<std::string> field_names;

};

struct Expr;
struct Expr {
    struct None {};
    struct Literal { std::string kind; std::string value; };
    struct Unary { Token op; std::shared_ptr<Expr> right; };
    struct Binary { std::shared_ptr<Expr> left; Token op; std::shared_ptr<Expr> right; };
    struct Grouping { std::shared_ptr<Expr> inner; };
    struct Variable { Token name; };
    struct Assign { Token name; std::shared_ptr<Expr> value; };
    struct Logical { std::shared_ptr<Expr> left; Token op; std::shared_ptr<Expr> right; };
    struct Call { std::shared_ptr<Expr> callee; Token paren; std::vector<Expr> args; std::vector<std::string> arg_names; };
    struct Index { std::shared_ptr<Expr> object; Token bracket; std::shared_ptr<Expr> index; };
    struct IndexSet { std::shared_ptr<Expr> object; Token bracket; std::shared_ptr<Expr> index; std::shared_ptr<Expr> value; };
    struct Vector { std::vector<Expr> elements; };
    struct Map { std::vector<Expr> keys; std::vector<Expr> values; };
    struct Get { std::shared_ptr<Expr> object; Token name; };
    struct Set { std::shared_ptr<Expr> object; Token name; std::shared_ptr<Expr> value; };
    struct StaticGet { std::shared_ptr<Expr> object; Token name; };
    struct This { Token keyword; };
    struct Cast { std::shared_ptr<Expr> expr; TypeNode target_type; };
    struct Throw { std::shared_ptr<Expr> expr; };
    struct Range { std::shared_ptr<Expr> start; std::shared_ptr<Expr> end; };
    struct Lambda { std::vector<Param> params; std::shared_ptr<Expr> body; };
    struct Own { std::shared_ptr<Expr> expr; };
    struct AddressOf { std::shared_ptr<Expr> expr; };
    struct BlockLambda { std::vector<Param> params; int64_t body_id; };

    std::string _tag;
    std::variant<Expr::None, Expr::Literal, Expr::Unary, Expr::Binary, Expr::Grouping, Expr::Variable, Expr::Assign, Expr::Logical, Expr::Call, Expr::Index, Expr::IndexSet, Expr::Vector, Expr::Map, Expr::Get, Expr::Set, Expr::StaticGet, Expr::This, Expr::Cast, Expr::Throw, Expr::Range, Expr::Lambda, Expr::Own, Expr::AddressOf, Expr::BlockLambda> _data;

    static Expr make_None() { return {"None", None{}}; }
    static Expr make_Literal(std::string kind, std::string value) { return {"Literal", Literal{kind, value}}; }
    static Expr make_Unary(Token op, Expr right) { return {"Unary", Unary{op, std::make_shared<Expr>(std::move(right))}}; }
    static Expr make_Binary(Expr left, Token op, Expr right) { return {"Binary", Binary{std::make_shared<Expr>(std::move(left)), op, std::make_shared<Expr>(std::move(right))}}; }
    static Expr make_Grouping(Expr inner) { return {"Grouping", Grouping{std::make_shared<Expr>(std::move(inner))}}; }
    static Expr make_Variable(Token name) { return {"Variable", Variable{name}}; }
    static Expr make_Assign(Token name, Expr value) { return {"Assign", Assign{name, std::make_shared<Expr>(std::move(value))}}; }
    static Expr make_Logical(Expr left, Token op, Expr right) { return {"Logical", Logical{std::make_shared<Expr>(std::move(left)), op, std::make_shared<Expr>(std::move(right))}}; }
    static Expr make_Call(Expr callee, Token paren, std::vector<Expr> args, std::vector<std::string> arg_names) { return {"Call", Call{std::make_shared<Expr>(std::move(callee)), paren, args, arg_names}}; }
    static Expr make_Index(Expr object, Token bracket, Expr index) { return {"Index", Index{std::make_shared<Expr>(std::move(object)), bracket, std::make_shared<Expr>(std::move(index))}}; }
    static Expr make_IndexSet(Expr object, Token bracket, Expr index, Expr value) { return {"IndexSet", IndexSet{std::make_shared<Expr>(std::move(object)), bracket, std::make_shared<Expr>(std::move(index)), std::make_shared<Expr>(std::move(value))}}; }
    static Expr make_Vector(std::vector<Expr> elements) { return {"Vector", Vector{elements}}; }
    static Expr make_Map(std::vector<Expr> keys, std::vector<Expr> values) { return {"Map", Map{keys, values}}; }
    static Expr make_Get(Expr object, Token name) { return {"Get", Get{std::make_shared<Expr>(std::move(object)), name}}; }
    static Expr make_Set(Expr object, Token name, Expr value) { return {"Set", Set{std::make_shared<Expr>(std::move(object)), name, std::make_shared<Expr>(std::move(value))}}; }
    static Expr make_StaticGet(Expr object, Token name) { return {"StaticGet", StaticGet{std::make_shared<Expr>(std::move(object)), name}}; }
    static Expr make_This(Token keyword) { return {"This", This{keyword}}; }
    static Expr make_Cast(Expr expr, TypeNode target_type) { return {"Cast", Cast{std::make_shared<Expr>(std::move(expr)), target_type}}; }
    static Expr make_Throw(Expr expr) { return {"Throw", Throw{std::make_shared<Expr>(std::move(expr))}}; }
    static Expr make_Range(Expr start, Expr end) { return {"Range", Range{std::make_shared<Expr>(std::move(start)), std::make_shared<Expr>(std::move(end))}}; }
    static Expr make_Lambda(std::vector<Param> params, Expr body) { return {"Lambda", Lambda{params, std::make_shared<Expr>(std::move(body))}}; }
    static Expr make_Own(Expr expr) { return {"Own", Own{std::make_shared<Expr>(std::move(expr))}}; }
    static Expr make_AddressOf(Expr expr) { return {"AddressOf", AddressOf{std::make_shared<Expr>(std::move(expr))}}; }
    static Expr make_BlockLambda(std::vector<Param> params, int64_t body_id) { return {"BlockLambda", BlockLambda{params, body_id}}; }

    std::string operator[](const std::string& key) const {
        if (key == "_tag") return _tag;
        return "";
    }
};

void print(const Expr& _v) { std::cout << "Expr(" << _v._tag << ")" << std::endl; }
std::string operator+(const std::string& _s, const Expr& _v) { return _s + _v._tag; }
std::string operator+(const Expr& _v, const std::string& _s) { return _v._tag + _s; }

struct ExternType {
    std::string lavina_name;
    std::string cpp_name;

};

struct ExternFn {
    std::string name;
    std::string cpp_name;
    TypeNode return_type;
    std::vector<Param> params;
    std::vector<Expr> param_defaults;

};

struct MatchArm {
    std::string pattern_name;
    std::vector<std::string> bindings;

};

struct Stmt;
struct Stmt {
    struct None {};
    struct ExprStmt { Expr expr; };
    struct Let { Token name; TypeNode var_type; Expr initializer; std::string visibility; bool is_ref; bool is_mut; };
    struct Const { Token name; TypeNode const_type; Expr value; std::string visibility; int64_t comptime_mode; };
    struct Return { Token keyword; Expr value; };
    struct If { Expr condition; std::shared_ptr<Stmt> then_branch; std::shared_ptr<Stmt> else_branch; };
    struct While { Expr condition; std::shared_ptr<Stmt> body; };
    struct For { Token item_name; Expr collection; std::shared_ptr<Stmt> body; bool is_ref; bool is_mut; };
    struct Block { std::vector<Stmt> statements; };
    struct Try { std::shared_ptr<Stmt> try_body; std::shared_ptr<Stmt> catch_body; std::string exception_name; };
    struct Function { Token name; std::vector<Param> params; TypeNode return_type; std::vector<Stmt> body; bool is_inline; int64_t comptime_mode; bool is_static; std::string visibility; std::vector<std::string> type_params; std::vector<Expr> param_defaults; };
    struct Class { Token name; std::vector<Stmt> body; std::string visibility; };
    struct Struct { Token name; std::vector<Stmt> body; std::string visibility; std::vector<std::string> type_params; };
    struct Enum { Token name; std::vector<EnumVariantNode> variants; std::vector<Stmt> methods; std::string visibility; std::vector<std::string> type_params; };
    struct Match { Expr expr; std::vector<MatchArm> arm_patterns; std::vector<Stmt> arm_bodies; };
    struct Namespace { Token name; std::vector<Stmt> body; std::string visibility; };
    struct Import { std::vector<Token> path; std::string alias; };
    struct Break { Token keyword; };
    struct Continue { Token keyword; };
    struct Pass { Token keyword; };
    struct CppBlock { std::string code; };
    struct Extern { std::string header; std::string import_path; std::string link_lib; std::vector<ExternType> types; std::vector<ExternFn> functions; };
    struct Extend { Token target_type; std::vector<Stmt> methods; std::string visibility; };

    std::string _tag;
    std::variant<Stmt::None, Stmt::ExprStmt, Stmt::Let, Stmt::Const, Stmt::Return, Stmt::If, Stmt::While, Stmt::For, Stmt::Block, Stmt::Try, Stmt::Function, Stmt::Class, Stmt::Struct, Stmt::Enum, Stmt::Match, Stmt::Namespace, Stmt::Import, Stmt::Break, Stmt::Continue, Stmt::Pass, Stmt::CppBlock, Stmt::Extern, Stmt::Extend> _data;

    static Stmt make_None() { return {"None", None{}}; }
    static Stmt make_ExprStmt(Expr expr) { return {"ExprStmt", ExprStmt{expr}}; }
    static Stmt make_Let(Token name, TypeNode var_type, Expr initializer, std::string visibility, bool is_ref, bool is_mut) { return {"Let", Let{name, var_type, initializer, visibility, is_ref, is_mut}}; }
    static Stmt make_Const(Token name, TypeNode const_type, Expr value, std::string visibility, int64_t comptime_mode) { return {"Const", Const{name, const_type, value, visibility, comptime_mode}}; }
    static Stmt make_Return(Token keyword, Expr value) { return {"Return", Return{keyword, value}}; }
    static Stmt make_If(Expr condition, Stmt then_branch, Stmt else_branch) { return {"If", If{condition, std::make_shared<Stmt>(std::move(then_branch)), std::make_shared<Stmt>(std::move(else_branch))}}; }
    static Stmt make_While(Expr condition, Stmt body) { return {"While", While{condition, std::make_shared<Stmt>(std::move(body))}}; }
    static Stmt make_For(Token item_name, Expr collection, Stmt body, bool is_ref, bool is_mut) { return {"For", For{item_name, collection, std::make_shared<Stmt>(std::move(body)), is_ref, is_mut}}; }
    static Stmt make_Block(std::vector<Stmt> statements) { return {"Block", Block{statements}}; }
    static Stmt make_Try(Stmt try_body, Stmt catch_body, std::string exception_name) { return {"Try", Try{std::make_shared<Stmt>(std::move(try_body)), std::make_shared<Stmt>(std::move(catch_body)), exception_name}}; }
    static Stmt make_Function(Token name, std::vector<Param> params, TypeNode return_type, std::vector<Stmt> body, bool is_inline, int64_t comptime_mode, bool is_static, std::string visibility, std::vector<std::string> type_params, std::vector<Expr> param_defaults) { return {"Function", Function{name, params, return_type, body, is_inline, comptime_mode, is_static, visibility, type_params, param_defaults}}; }
    static Stmt make_Class(Token name, std::vector<Stmt> body, std::string visibility) { return {"Class", Class{name, body, visibility}}; }
    static Stmt make_Struct(Token name, std::vector<Stmt> body, std::string visibility, std::vector<std::string> type_params) { return {"Struct", Struct{name, body, visibility, type_params}}; }
    static Stmt make_Enum(Token name, std::vector<EnumVariantNode> variants, std::vector<Stmt> methods, std::string visibility, std::vector<std::string> type_params) { return {"Enum", Enum{name, variants, methods, visibility, type_params}}; }
    static Stmt make_Match(Expr expr, std::vector<MatchArm> arm_patterns, std::vector<Stmt> arm_bodies) { return {"Match", Match{expr, arm_patterns, arm_bodies}}; }
    static Stmt make_Namespace(Token name, std::vector<Stmt> body, std::string visibility) { return {"Namespace", Namespace{name, body, visibility}}; }
    static Stmt make_Import(std::vector<Token> path, std::string alias) { return {"Import", Import{path, alias}}; }
    static Stmt make_Break(Token keyword) { return {"Break", Break{keyword}}; }
    static Stmt make_Continue(Token keyword) { return {"Continue", Continue{keyword}}; }
    static Stmt make_Pass(Token keyword) { return {"Pass", Pass{keyword}}; }
    static Stmt make_CppBlock(std::string code) { return {"CppBlock", CppBlock{code}}; }
    static Stmt make_Extern(std::string header, std::string import_path, std::string link_lib, std::vector<ExternType> types, std::vector<ExternFn> functions) { return {"Extern", Extern{header, import_path, link_lib, types, functions}}; }
    static Stmt make_Extend(Token target_type, std::vector<Stmt> methods, std::string visibility) { return {"Extend", Extend{target_type, methods, visibility}}; }

    std::string operator[](const std::string& key) const {
        if (key == "_tag") return _tag;
        return "";
    }
};

void print(const Stmt& _v) { std::cout << "Stmt(" << _v._tag << ")" << std::endl; }
std::string operator+(const std::string& _s, const Stmt& _v) { return _s + _v._tag; }
std::string operator+(const Stmt& _v, const std::string& _s) { return _v._tag + _s; }

bool is_integer_type(const TypeNode& t) {
    {
        const auto& _match_0 = t;
        if (std::holds_alternative<std::decay_t<decltype(_match_0)>::Int>(_match_0._data)) {
            return true;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_0)>::Int8>(_match_0._data)) {
            return true;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_0)>::Int16>(_match_0._data)) {
            return true;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_0)>::Int32>(_match_0._data)) {
            return true;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_0)>::USize>(_match_0._data)) {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

bool is_float_type(const TypeNode& t) {
    {
        const auto& _match_1 = t;
        if (std::holds_alternative<std::decay_t<decltype(_match_1)>::Float>(_match_1._data)) {
            return true;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_1)>::Float32>(_match_1._data)) {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

bool is_string_type(const TypeNode& t) {
    {
        const auto& _match_2 = t;
        if (std::holds_alternative<std::decay_t<decltype(_match_2)>::Str>(_match_2._data)) {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

bool is_bytes_type(const TypeNode& t) {
    {
        const auto& _match_3 = t;
        if (std::holds_alternative<std::decay_t<decltype(_match_3)>::Bytes>(_match_3._data)) {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

std::string type_to_cpp(const TypeNode& t) {
    {
        const auto& _match_4 = t;
        if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Int>(_match_4._data)) {
            return std::string("int64_t");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Float>(_match_4._data)) {
            return std::string("double");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Str>(_match_4._data)) {
            return std::string("std::string");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Bool>(_match_4._data)) {
            return std::string("bool");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Void>(_match_4._data)) {
            return std::string("void");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Auto>(_match_4._data)) {
            return std::string("auto");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::NullType>(_match_4._data)) {
            return std::string("std::nullptr_t");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Custom>(_match_4._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_4)>::Custom>(_match_4._data);
            auto& name = _v.name;
            auto& type_args = _v.type_args;
            if ((static_cast<int64_t>(type_args.size()) > INT64_C(0))) {
                std::vector<std::string> ta = {};
                for (const auto& a : type_args) {
                    ta.push_back(type_to_cpp(a));
                }
                return ((((std::string("") + (name)) + std::string("<")) + (lv_join(ta, std::string(", ")))) + std::string(">"));
            }
            return name;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Array>(_match_4._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_4)>::Array>(_match_4._data);
            auto& inner = *_v.inner;
            return ((std::string("std::vector<") + (type_to_cpp(inner))) + std::string(">"));
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::HashSet>(_match_4._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_4)>::HashSet>(_match_4._data);
            auto& inner = *_v.inner;
            return ((std::string("std::unordered_set<") + (type_to_cpp(inner))) + std::string(">"));
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::HashMap>(_match_4._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_4)>::HashMap>(_match_4._data);
            auto& key_type = *_v.key_type;
            auto& value_type = *_v.value_type;
            return ((((std::string("std::unordered_map<") + (type_to_cpp(key_type))) + std::string(", ")) + (type_to_cpp(value_type))) + std::string(">"));
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Nullable>(_match_4._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_4)>::Nullable>(_match_4._data);
            auto& inner = *_v.inner;
            return ((std::string("std::optional<") + (type_to_cpp(inner))) + std::string(">"));
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Int8>(_match_4._data)) {
            return std::string("int8_t");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Int16>(_match_4._data)) {
            return std::string("int16_t");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Int32>(_match_4._data)) {
            return std::string("int32_t");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Float32>(_match_4._data)) {
            return std::string("float");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::USize>(_match_4._data)) {
            return std::string("size_t");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::CString>(_match_4._data)) {
            return std::string("const char*");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Ptr>(_match_4._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_4)>::Ptr>(_match_4._data);
            auto& inner = *_v.inner;
            return ((std::string("") + (type_to_cpp(inner))) + std::string("*"));
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_4)>::Bytes>(_match_4._data)) {
            return std::string("std::vector<uint8_t>");
        }
        else {
            return std::string("auto");
        }
    }
    return std::string("auto");
}

std::string type_to_display(const TypeNode& t) {
    {
        const auto& _match_5 = t;
        if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Int>(_match_5._data)) {
            return std::string("int");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Float>(_match_5._data)) {
            return std::string("float");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Str>(_match_5._data)) {
            return std::string("string");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Bool>(_match_5._data)) {
            return std::string("bool");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Void>(_match_5._data)) {
            return std::string("void");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Auto>(_match_5._data)) {
            return std::string("auto");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::NullType>(_match_5._data)) {
            return std::string("null");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Custom>(_match_5._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_5)>::Custom>(_match_5._data);
            auto& name = _v.name;
            auto& _ = _v.type_args;
            return name;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Array>(_match_5._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_5)>::Array>(_match_5._data);
            auto& inner = *_v.inner;
            return ((std::string("vector[") + (type_to_display(inner))) + std::string("]"));
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::HashSet>(_match_5._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_5)>::HashSet>(_match_5._data);
            auto& inner = *_v.inner;
            return ((std::string("set[") + (type_to_display(inner))) + std::string("]"));
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::HashMap>(_match_5._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_5)>::HashMap>(_match_5._data);
            auto& k = *_v.key_type;
            auto& v = *_v.value_type;
            return ((((std::string("map[") + (type_to_display(k))) + std::string(", ")) + (type_to_display(v))) + std::string("]"));
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Nullable>(_match_5._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_5)>::Nullable>(_match_5._data);
            auto& inner = *_v.inner;
            return ((std::string("") + (type_to_display(inner))) + std::string("?"));
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Int8>(_match_5._data)) {
            return std::string("int8");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Int16>(_match_5._data)) {
            return std::string("int16");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Int32>(_match_5._data)) {
            return std::string("int32");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Float32>(_match_5._data)) {
            return std::string("float32");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::USize>(_match_5._data)) {
            return std::string("usize");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::CString>(_match_5._data)) {
            return std::string("cstring");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Ptr>(_match_5._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_5)>::Ptr>(_match_5._data);
            auto& inner = *_v.inner;
            return ((std::string("ptr[") + (type_to_display(inner))) + std::string("]"));
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_5)>::Bytes>(_match_5._data)) {
            return std::string("bytes");
        }
        else {
            return std::string("unknown");
        }
    }
    return std::string("unknown");
}

std::string find_enum_for_variant(std::unordered_map<std::string, std::vector<EnumVariantNode>>& known_enums, const std::string& variant_name) {
    std::string found = std::string("");
    int64_t count = INT64_C(0);
    std::vector<std::string> keys = lv_keys(known_enums);
    for (const auto& key : keys) {
        std::vector<EnumVariantNode> variants = known_enums[key];
        for (const auto& v : variants) {
            if ((v.name.lexeme == variant_name)) {
                if ((count == INT64_C(0))) {
                    found = key;
                }
                count = (count + INT64_C(1));
            }
        }
    }
    if ((count == INT64_C(1))) {
        return found;
    }
    return std::string("");
}

struct CppCodegen {
    std::string output;
    std::string declarations;
    int64_t indent_level;
    int64_t temp_counter;
    bool has_main;
    std::unordered_map<std::string, std::vector<EnumVariantNode>> known_enums;
    std::vector<std::string> extern_includes;
    std::vector<std::string> extern_link_libs;
    std::vector<std::string> extern_import_paths;
    std::unordered_map<std::string, std::string> extern_fn_names;
    std::unordered_map<std::string, std::string> extern_type_names;
    std::unordered_map<std::string, std::vector<Param>> extern_fn_params;
    std::unordered_map<std::string, TypeNode> var_types;
    std::vector<std::string> module_short_names;
    std::vector<std::string> module_full_names;
    std::vector<std::string> module_aliases;
    std::vector<std::vector<Stmt>> module_stmts;
    std::vector<std::vector<Stmt>> lambda_blocks;
    std::unordered_map<std::string, std::vector<Stmt>> extend_methods;
    bool in_extend;
    std::unordered_map<std::string, std::vector<Param>> fn_params;
    std::unordered_map<std::string, std::vector<Expr>> fn_defaults;

    CppCodegen() {
        this->output = std::string("");
        this->declarations = std::string("");
        this->indent_level = INT64_C(0);
        this->temp_counter = INT64_C(0);
        this->has_main = false;
        this->known_enums = {{}};
        this->extern_includes = {};
        this->extern_link_libs = {};
        this->extern_import_paths = {};
        this->extern_fn_names = {{}};
        this->extern_type_names = {{}};
        this->var_types = {{}};
        this->extern_fn_params = {{}};
        this->module_short_names = {};
        this->module_full_names = {};
        this->module_aliases = {};
        this->module_stmts = {};
        this->lambda_blocks = {};
        this->extend_methods = {{}};
        this->in_extend = false;
        this->fn_params = {{}};
        this->fn_defaults = {{}};
    }

    void set_modules(const std::vector<std::string>& short_names, const std::vector<std::string>& full_names, const std::vector<std::string>& aliases, const std::vector<std::vector<Stmt>>& stmts) {
        this->module_short_names = short_names;
        this->module_full_names = full_names;
        this->module_aliases = aliases;
        this->module_stmts = stmts;
    }

    std::string indent() {
        std::string result = std::string("");
        for (int64_t i = INT64_C(0); i < this->indent_level; i++) {
            result = (result + std::string("    "));
        }
        return result;
    }

    EnumVariantNode get_variant_info(const std::string& enum_name, const std::string& variant_name) {
        if ((this->known_enums.count(enum_name) > 0)) {
            std::vector<EnumVariantNode> variants = this->known_enums[enum_name];
            for (const auto& v : variants) {
                if ((v.name.lexeme == variant_name)) {
                    return v;
                }
            }
        }
        std::vector<TypeNode> empty_types = {};
        std::vector<std::string> empty_names = {};
        return EnumVariantNode(Token(std::string(""), std::string(""), INT64_C(0), INT64_C(0)), empty_types, empty_names);
    }

    bool is_known_enum(const std::string& name) {
        if ((this->known_enums.count(name) > 0)) {
            return true;
        }
        int64_t angle = lv_index_of(name, std::string("<"));
        if ((angle >= INT64_C(0))) {
            std::string base = name.substr(INT64_C(0), (angle) - (INT64_C(0)));
            return (this->known_enums.count(base) > 0);
        }
        return false;
    }

    std::string template_prefix(const std::vector<std::string>& type_params) {
        if ((static_cast<int64_t>(type_params.size()) == INT64_C(0))) {
            return std::string("");
        }
        std::vector<std::string> tps = {};
        for (const auto& tp : type_params) {
            tps.push_back(((std::string("typename ") + (tp)) + std::string("")));
        }
        return ((((std::string("") + ((*this).indent())) + std::string("template<")) + (lv_join(tps, std::string(", ")))) + std::string(">\n"));
    }

    void push_unique(std::vector<std::string>& vec, std::string value) {
        for (const auto& existing : vec) {
            if ((existing == value)) {
                return;
            }
        }
        vec.push_back(value);
    }

    std::string wrap_convert(std::string expr, const TypeNode& from, const TypeNode& expected) {
        {
            const auto& _match_6 = from;
            if (std::holds_alternative<std::decay_t<decltype(_match_6)>::Str>(_match_6._data)) {
                {
                    const auto& _match_7 = expected;
                    if (std::holds_alternative<std::decay_t<decltype(_match_7)>::CString>(_match_7._data)) {
                        return ((std::string("(") + (expr)) + std::string(").c_str()"));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_6)>::CString>(_match_6._data)) {
                {
                    const auto& _match_8 = expected;
                    if (std::holds_alternative<std::decay_t<decltype(_match_8)>::Str>(_match_8._data)) {
                        return ((std::string("std::string(") + (expr)) + std::string(")"));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_6)>::Int>(_match_6._data)) {
                {
                    const auto& _match_9 = expected;
                    if (std::holds_alternative<std::decay_t<decltype(_match_9)>::Int32>(_match_9._data)) {
                        return ((std::string("static_cast<int32_t>(") + (expr)) + std::string(")"));
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_9)>::USize>(_match_9._data)) {
                        return ((std::string("static_cast<size_t>(") + (expr)) + std::string(")"));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_6)>::Int32>(_match_6._data)) {
                {
                    const auto& _match_10 = expected;
                    if (std::holds_alternative<std::decay_t<decltype(_match_10)>::Int>(_match_10._data)) {
                        return ((std::string("static_cast<int64_t>(") + (expr)) + std::string(")"));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_6)>::USize>(_match_6._data)) {
                {
                    const auto& _match_11 = expected;
                    if (std::holds_alternative<std::decay_t<decltype(_match_11)>::Int>(_match_11._data)) {
                        return ((std::string("static_cast<int64_t>(") + (expr)) + std::string(")"));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_6)>::Float>(_match_6._data)) {
                {
                    const auto& _match_12 = expected;
                    if (std::holds_alternative<std::decay_t<decltype(_match_12)>::Float32>(_match_12._data)) {
                        return ((std::string("static_cast<float>(") + (expr)) + std::string(")"));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_6)>::Float32>(_match_6._data)) {
                {
                    const auto& _match_13 = expected;
                    if (std::holds_alternative<std::decay_t<decltype(_match_13)>::Float>(_match_13._data)) {
                        return ((std::string("static_cast<double>(") + (expr)) + std::string(")"));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else {
                /* pass */
            }
        }
        return expr;
    }

    std::string wrap_extern_arg(std::string expr, const Expr& original, const TypeNode& expected) {
        {
            const auto& _match_14 = original;
            if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Literal>(_match_14._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_14)>::Literal>(_match_14._data);
                auto& kind = _v.kind;
                auto& value = _v.value;
                if ((kind == std::string("null"))) {
                    {
                        const auto& _match_15 = expected;
                        if (std::holds_alternative<std::decay_t<decltype(_match_15)>::Ptr>(_match_15._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_15)>::Ptr>(_match_15._data);
                            auto& inner = *_v.inner;
                            return std::string("nullptr");
                        }
                        else {
                            /* pass */
                        }
                    }
                }
                if ((kind == std::string("string"))) {
                    {
                        const auto& _match_16 = expected;
                        if (std::holds_alternative<std::decay_t<decltype(_match_16)>::CString>(_match_16._data)) {
                            return ((std::string("(") + (expr)) + std::string(").c_str()"));
                        }
                        else {
                            /* pass */
                        }
                    }
                }
                if ((kind == std::string("int"))) {
                    {
                        const auto& _match_17 = expected;
                        if (std::holds_alternative<std::decay_t<decltype(_match_17)>::Int32>(_match_17._data)) {
                            return ((std::string("static_cast<int32_t>(") + (expr)) + std::string(")"));
                        }
                        else if (std::holds_alternative<std::decay_t<decltype(_match_17)>::USize>(_match_17._data)) {
                            return ((std::string("static_cast<size_t>(") + (expr)) + std::string(")"));
                        }
                        else {
                            /* pass */
                        }
                    }
                }
                if ((kind == std::string("float"))) {
                    {
                        const auto& _match_18 = expected;
                        if (std::holds_alternative<std::decay_t<decltype(_match_18)>::Float32>(_match_18._data)) {
                            return ((std::string("static_cast<float>(") + (expr)) + std::string(")"));
                        }
                        else {
                            /* pass */
                        }
                    }
                }
                return expr;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_14)>::Variable>(_match_14._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_14)>::Variable>(_match_14._data);
                auto& tok = _v.name;
                if ((this->var_types.count(tok.lexeme) > 0)) {
                    TypeNode vt = this->var_types[tok.lexeme];
                    return (*this).wrap_convert(expr, vt, expected);
                }
            }
            else {
                /* pass */
            }
        }
        return expr;
    }

    std::string emit_type(const TypeNode& t) {
        {
            const auto& _match_19 = t;
            if (std::holds_alternative<std::decay_t<decltype(_match_19)>::Custom>(_match_19._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_19)>::Custom>(_match_19._data);
                auto& name = _v.name;
                auto& type_args = _v.type_args;
                if ((static_cast<int64_t>(type_args.size()) > INT64_C(0))) {
                    std::vector<std::string> ta = {};
                    for (const auto& a : type_args) {
                        ta.push_back((*this).emit_type(a));
                    }
                    return ((((std::string("") + (name)) + std::string("<")) + (lv_join(ta, std::string(", ")))) + std::string(">"));
                }
                if ((this->extern_type_names.count(name) > 0)) {
                    return this->extern_type_names[name];
                }
                return name;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_19)>::Array>(_match_19._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_19)>::Array>(_match_19._data);
                auto& inner = *_v.inner;
                return ((std::string("std::vector<") + ((*this).emit_type(inner))) + std::string(">"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_19)>::HashSet>(_match_19._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_19)>::HashSet>(_match_19._data);
                auto& inner = *_v.inner;
                return ((std::string("std::unordered_set<") + ((*this).emit_type(inner))) + std::string(">"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_19)>::HashMap>(_match_19._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_19)>::HashMap>(_match_19._data);
                auto& key_type = *_v.key_type;
                auto& value_type = *_v.value_type;
                return ((((std::string("std::unordered_map<") + ((*this).emit_type(key_type))) + std::string(", ")) + ((*this).emit_type(value_type))) + std::string(">"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_19)>::Nullable>(_match_19._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_19)>::Nullable>(_match_19._data);
                auto& inner = *_v.inner;
                return ((std::string("std::optional<") + ((*this).emit_type(inner))) + std::string(">"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_19)>::Ptr>(_match_19._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_19)>::Ptr>(_match_19._data);
                auto& inner = *_v.inner;
                return ((std::string("") + ((*this).emit_type(inner))) + std::string("*"));
            }
            else {
                return type_to_cpp(t);
            }
        }
    }

    std::string token_to_cpp_op(Token t) {
        if ((t.token_type == TK_PLUS)) {
            return std::string("+");
        }
        else {
            if ((t.token_type == TK_MINUS)) {
                return std::string("-");
            }
            else {
                if ((t.token_type == TK_STAR)) {
                    return std::string("*");
                }
                else {
                    if ((t.token_type == TK_SLASH)) {
                        return std::string("/");
                    }
                    else {
                        if ((t.token_type == TK_PERCENT)) {
                            return std::string("%");
                        }
                        else {
                            if ((t.token_type == TK_EQUAL_EQUAL)) {
                                return std::string("==");
                            }
                            else {
                                if ((t.token_type == TK_BANG_EQUAL)) {
                                    return std::string("!=");
                                }
                                else {
                                    if ((t.token_type == TK_LESS)) {
                                        return std::string("<");
                                    }
                                    else {
                                        if ((t.token_type == TK_LESS_EQUAL)) {
                                            return std::string("<=");
                                        }
                                        else {
                                            if ((t.token_type == TK_GREATER)) {
                                                return std::string(">");
                                            }
                                            else {
                                                if ((t.token_type == TK_GREATER_EQUAL)) {
                                                    return std::string(">=");
                                                }
                                                else {
                                                    if ((t.token_type == TK_BANG) || (t.token_type == TK_NOT)) {
                                                        return std::string("!");
                                                    }
                                                    else {
                                                        if ((t.token_type == TK_AND)) {
                                                            return std::string("&&");
                                                        }
                                                        else {
                                                            if ((t.token_type == TK_OR)) {
                                                                return std::string("||");
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return std::string("/* unknown op */");
    }

    std::string default_init(std::string cpp_type) {
        if ((cpp_type == std::string("int64_t"))) {
            return std::string(" = 0");
        }
        else {
            if ((cpp_type == std::string("double"))) {
                return std::string(" = 0.0");
            }
            else {
                if ((cpp_type == std::string("bool"))) {
                    return std::string(" = false");
                }
                else {
                    if ((cpp_type == std::string("std::string"))) {
                        return std::string(" = \"\"");
                    }
                    else {
                        if ((cpp_type == std::string("int8_t"))) {
                            return std::string(" = 0");
                        }
                        else {
                            if ((cpp_type == std::string("int16_t"))) {
                                return std::string(" = 0");
                            }
                            else {
                                if ((cpp_type == std::string("int32_t"))) {
                                    return std::string(" = 0");
                                }
                                else {
                                    if ((cpp_type == std::string("size_t"))) {
                                        return std::string(" = 0");
                                    }
                                    else {
                                        if ((cpp_type == std::string("float"))) {
                                            return std::string(" = 0.0f");
                                        }
                                        else {
                                            if ((cpp_type == std::string("const char*"))) {
                                                return std::string(" = nullptr");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return std::string("");
    }

    std::string emit_cast(const Expr& expr, const TypeNode& target_type, bool m) {
        std::string ex = (*this).emit_expr(expr, m);
        std::string t = (*this).emit_type(target_type);
        TypeNode src = (*this).infer_source_type(expr);
        if (is_string_type(src)) {
            if (is_integer_type(target_type)) {
                {
                    const auto& _match_20 = target_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_20)>::Int>(_match_20._data)) {
                        return ((std::string("__str_to_int(") + (ex)) + std::string(")"));
                    }
                    else {
                        return ((((std::string("static_cast<") + (t)) + std::string(">(__str_to_int(")) + (ex)) + std::string("))"));
                    }
                }
            }
            if (is_float_type(target_type)) {
                {
                    const auto& _match_21 = target_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_21)>::Float>(_match_21._data)) {
                        return ((std::string("__str_to_float(") + (ex)) + std::string(")"));
                    }
                    else {
                        return ((((std::string("static_cast<") + (t)) + std::string(">(__str_to_float(")) + (ex)) + std::string("))"));
                    }
                }
            }
            if (is_bytes_type(target_type)) {
                return ((std::string("__bytes_from_string(") + (ex)) + std::string(")"));
            }
        }
        if (is_string_type(target_type)) {
            if (is_integer_type(src) || is_float_type(src)) {
                return ((std::string("to_string(") + (ex)) + std::string(")"));
            }
            {
                const auto& _match_22 = src;
                if (std::holds_alternative<std::decay_t<decltype(_match_22)>::Bool>(_match_22._data)) {
                    return ((std::string("to_string(") + (ex)) + std::string(")"));
                }
                else {
                    /* pass */
                }
            }
        }
        if (is_bytes_type(src) && is_string_type(target_type)) {
            return ((std::string("__bytes_to_string(") + (ex)) + std::string(")"));
        }
        if (is_string_type(src) && is_bytes_type(target_type)) {
            return ((std::string("__bytes_from_string(") + (ex)) + std::string(")"));
        }
        return ((((std::string("static_cast<") + (t)) + std::string(">(")) + (ex)) + std::string(")"));
    }

    std::string emit_member_prefix(const Expr& object, bool m) {
        if (this->in_extend) {
            {
                const auto& _match_23 = object;
                if (std::holds_alternative<std::decay_t<decltype(_match_23)>::This>(_match_23._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_23)>::This>(_match_23._data);
                    auto& kw = _v.keyword;
                    return std::string("self.");
                }
                else {
                    return ((std::string("") + ((*this).emit_expr(object, m))) + std::string("."));
                }
            }
        }
        if (m) {
            {
                const auto& _match_24 = object;
                if (std::holds_alternative<std::decay_t<decltype(_match_24)>::This>(_match_24._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_24)>::This>(_match_24._data);
                    auto& kw = _v.keyword;
                    return std::string("this->");
                }
                else {
                    return ((std::string("") + ((*this).emit_expr(object, m))) + std::string("."));
                }
            }
        }
        return ((std::string("") + ((*this).emit_expr(object, m))) + std::string("."));
    }

    std::string emit_expr(const Expr& e, bool m) {
        {
            const auto& _match_25 = e;
            if (_match_25._tag == "None") {
                return std::string("");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Literal>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Literal>(_match_25._data);
                auto& kind = _v.kind;
                auto& value = _v.value;
                return (*this).emit_literal(kind, value);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Unary>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Unary>(_match_25._data);
                auto& op = _v.op;
                auto& right = *_v.right;
                std::string r = (*this).emit_expr(right, m);
                std::string op_str = (*this).token_to_cpp_op(op);
                return ((((std::string("(") + (op_str)) + std::string("")) + (r)) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Binary>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Binary>(_match_25._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                std::string l = (*this).emit_expr(left, m);
                std::string r = (*this).emit_expr(right, m);
                std::string op_str = (*this).token_to_cpp_op(op);
                return ((((((std::string("(") + (l)) + std::string(" ")) + (op_str)) + std::string(" ")) + (r)) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Grouping>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Grouping>(_match_25._data);
                auto& inner = *_v.inner;
                return ((std::string("(") + ((*this).emit_expr(inner, m))) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Variable>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Variable>(_match_25._data);
                auto& name = _v.name;
                return name.lexeme;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Assign>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Assign>(_match_25._data);
                auto& name = _v.name;
                auto& value = *_v.value;
                return ((((std::string("") + (name.lexeme)) + std::string(" = ")) + ((*this).emit_expr(value, m))) + std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Logical>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Logical>(_match_25._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                std::string l = (*this).emit_expr(left, m);
                std::string r = (*this).emit_expr(right, m);
                std::string op_str = std::string("&&");
                if ((op.token_type == TK_OR)) {
                    op_str = std::string("||");
                }
                return ((((((std::string("") + (l)) + std::string(" ")) + (op_str)) + std::string(" ")) + (r)) + std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Call>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Call>(_match_25._data);
                auto& callee = *_v.callee;
                auto& paren = _v.paren;
                auto& args = _v.args;
                auto& arg_names = _v.arg_names;
                return (*this).emit_call_expr(callee, args, arg_names, m);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Index>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Index>(_match_25._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                return ((((std::string("") + ((*this).emit_expr(object, m))) + std::string("[")) + ((*this).emit_expr(index, m))) + std::string("]"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::IndexSet>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::IndexSet>(_match_25._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                auto& value = *_v.value;
                return ((((((std::string("") + ((*this).emit_expr(object, m))) + std::string("[")) + ((*this).emit_expr(index, m))) + std::string("] = ")) + ((*this).emit_expr(value, m))) + std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Vector>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Vector>(_match_25._data);
                auto& elements = _v.elements;
                if ((static_cast<int64_t>(elements.size()) == INT64_C(0))) {
                    return std::string("{}");
                }
                std::vector<std::string> elems = {};
                for (const auto& el : elements) {
                    elems.push_back((*this).emit_expr(el, m));
                }
                return ((std::string("std::vector{") + (lv_join(elems, std::string(", ")))) + std::string("}"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Map>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Map>(_match_25._data);
                auto& keys = _v.keys;
                auto& values = _v.values;
                std::vector<std::string> entries = {};
                for (int64_t i = INT64_C(0); i < static_cast<int64_t>(keys.size()); i++) {
                    entries.push_back(((((std::string("{") + ((*this).emit_expr(keys[i], m))) + std::string(", ")) + ((*this).emit_expr(values[i], m))) + std::string("}")));
                }
                return ((std::string("{{") + (lv_join(entries, std::string(", ")))) + std::string("}}"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Get>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Get>(_match_25._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                return ((((std::string("") + ((*this).emit_member_prefix(object, m))) + std::string("")) + (name.lexeme)) + std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Set>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Set>(_match_25._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                auto& value = *_v.value;
                return ((((((std::string("") + ((*this).emit_member_prefix(object, m))) + std::string("")) + (name.lexeme)) + std::string(" = ")) + ((*this).emit_expr(value, m))) + std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::StaticGet>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::StaticGet>(_match_25._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                if (m) {
                    {
                        const auto& _match_26 = object;
                        if (std::holds_alternative<std::decay_t<decltype(_match_26)>::This>(_match_26._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_26)>::This>(_match_26._data);
                            auto& kw = _v.keyword;
                            return ((std::string("(*this)::") + (name.lexeme)) + std::string(""));
                        }
                        else {
                            return ((((std::string("") + ((*this).emit_expr(object, m))) + std::string("::")) + (name.lexeme)) + std::string(""));
                        }
                    }
                }
                return ((((std::string("") + ((*this).emit_expr(object, m))) + std::string("::")) + (name.lexeme)) + std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::This>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::This>(_match_25._data);
                auto& keyword = _v.keyword;
                if (this->in_extend) {
                    return std::string("self");
                }
                return std::string("(*this)");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Cast>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Cast>(_match_25._data);
                auto& expr = *_v.expr;
                auto& target_type = _v.target_type;
                return (*this).emit_cast(expr, target_type, m);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Throw>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Throw>(_match_25._data);
                auto& expr = *_v.expr;
                return ((std::string("throw std::runtime_error(") + ((*this).emit_expr(expr, m))) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Lambda>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Lambda>(_match_25._data);
                auto& params = _v.params;
                auto& body = *_v.body;
                return (*this).emit_lambda(params, body, m);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::Own>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::Own>(_match_25._data);
                auto& expr = *_v.expr;
                return ((std::string("std::move(") + ((*this).emit_expr(expr, m))) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::AddressOf>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::AddressOf>(_match_25._data);
                auto& expr = *_v.expr;
                return ((std::string("&(") + ((*this).emit_expr(expr, m))) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_25)>::BlockLambda>(_match_25._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_25)>::BlockLambda>(_match_25._data);
                auto& params = _v.params;
                auto& body_id = _v.body_id;
                return (*this).emit_block_lambda(params, body_id, m);
            }
            else {
                return std::string("");
            }
        }
    }

    std::string emit_literal(std::string kind, std::string value) {
        if ((kind == std::string("int"))) {
            return ((std::string("INT64_C(") + (value)) + std::string(")"));
        }
        else {
            if ((kind == std::string("float"))) {
                return value;
            }
            else {
                if ((kind == std::string("string"))) {
                    return ((std::string("std::string(\"") + (value)) + std::string("\")"));
                }
                else {
                    if ((kind == std::string("bool"))) {
                        return value;
                    }
                    else {
                        if ((kind == std::string("null"))) {
                            return std::string("std::nullopt");
                        }
                    }
                }
            }
        }
        return value;
    }

    std::vector<std::string> resolve_named_args(const std::vector<Expr>& args, const std::vector<std::string>& arg_names, const std::vector<Param>& params, const std::vector<Expr>& defaults, bool in_method) {
        bool has_named = false;
        for (const auto& n : arg_names) {
            if ((n != std::string(""))) {
                has_named = true;
            }
        }
        if ((!has_named)) {
            std::vector<std::string> result = {};
            for (const auto& a : args) {
                result.push_back((*this).emit_expr(a, in_method));
            }
            int64_t i = static_cast<int64_t>(args.size());
            while ((i < static_cast<int64_t>(params.size()))) {
                if ((i < static_cast<int64_t>(defaults.size()))) {
                    result.push_back((*this).emit_expr(defaults[i], in_method));
                }
                i = (i + INT64_C(1));
            }
            return result;
        }
        int64_t positional_count = INT64_C(0);
        int64_t ai = INT64_C(0);
        while ((ai < static_cast<int64_t>(arg_names.size()))) {
            if ((arg_names[ai] != std::string(""))) {
                ai = static_cast<int64_t>(arg_names.size());
            }
            else {
                positional_count = (positional_count + INT64_C(1));
                ai = (ai + INT64_C(1));
            }
        }
        std::vector<std::string> result = {};
        int64_t pi = INT64_C(0);
        while ((pi < static_cast<int64_t>(params.size()))) {
            if ((pi < positional_count)) {
                result.push_back((*this).emit_expr(args[pi], in_method));
            }
            else {
                bool found = false;
                int64_t ni = positional_count;
                while ((ni < static_cast<int64_t>(arg_names.size()))) {
                    if ((arg_names[ni] == params[pi].name.lexeme)) {
                        result.push_back((*this).emit_expr(args[ni], in_method));
                        found = true;
                        ni = static_cast<int64_t>(arg_names.size());
                    }
                    else {
                        ni = (ni + INT64_C(1));
                    }
                }
                if ((!found)) {
                    if ((pi < static_cast<int64_t>(defaults.size()))) {
                        result.push_back((*this).emit_expr(defaults[pi], in_method));
                    }
                }
            }
            pi = (pi + INT64_C(1));
        }
        return result;
    }

    std::vector<std::string> emit_args(const std::vector<Expr>& args, bool m) {
        std::vector<std::string> result = {};
        for (const auto& a : args) {
            result.push_back((*this).emit_expr(a, m));
        }
        return result;
    }

    void apply_extern_wrapping(std::vector<std::string>& arg_strs, const std::vector<Expr>& args, const std::string& fn_name) {
        if ((this->extern_fn_params.count(fn_name) > 0)) {
            std::vector<Param> eparams = this->extern_fn_params[fn_name];
            for (int64_t i = INT64_C(0); i < static_cast<int64_t>(arg_strs.size()); i++) {
                if ((i < static_cast<int64_t>(eparams.size()))) {
                    arg_strs[i] = (*this).wrap_extern_arg(arg_strs[i], args[i], eparams[i].param_type);
                }
            }
        }
    }

    std::string resolve_fn_name(const std::string& name) {
        if ((this->extern_fn_names.count(name) > 0)) {
            return this->extern_fn_names[name];
        }
        return name;
    }

    std::string emit_call_expr(const Expr& callee, const std::vector<Expr>& args, const std::vector<std::string>& arg_names, bool in_method) {
        {
            const auto& _match_27 = callee;
            if (std::holds_alternative<std::decay_t<decltype(_match_27)>::Get>(_match_27._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_27)>::Get>(_match_27._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                std::string obj = (*this).emit_expr(object, in_method);
                std::vector<std::string> arg_strs = (*this).emit_args(args, in_method);
                std::string remapped = (*this).try_remap_method(obj, name.lexeme, arg_strs);
                if ((remapped != std::string(""))) {
                    return remapped;
                }
                std::string ext_call = (*this).try_extend_method(object, name.lexeme, obj, arg_strs);
                if ((ext_call != std::string(""))) {
                    return ext_call;
                }
                return ((((((std::string("") + (obj)) + std::string(".")) + (name.lexeme)) + std::string("(")) + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_27)>::StaticGet>(_match_27._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_27)>::StaticGet>(_match_27._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                std::string obj = (*this).emit_expr(object, in_method);
                std::vector<std::string> arg_strs = (*this).emit_args(args, in_method);
                {
                    const auto& _match_28 = object;
                    if (std::holds_alternative<std::decay_t<decltype(_match_28)>::Variable>(_match_28._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_28)>::Variable>(_match_28._data);
                        auto& tok = _v.name;
                        if ((*this).is_known_enum(tok.lexeme)) {
                            return ((((((std::string("") + (obj)) + std::string("::make_")) + (name.lexeme)) + std::string("(")) + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
                        }
                    }
                    else {
                        /* pass */
                    }
                }
                return ((((((std::string("") + (obj)) + std::string("::")) + (name.lexeme)) + std::string("(")) + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_27)>::Variable>(_match_27._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_27)>::Variable>(_match_27._data);
                auto& tok = _v.name;
                if ((this->fn_params.count(tok.lexeme) > 0)) {
                    std::vector<Param> fparams = this->fn_params[tok.lexeme];
                    std::vector<Expr> fdefaults = {};
                    if ((this->fn_defaults.count(tok.lexeme) > 0)) {
                        fdefaults = this->fn_defaults[tok.lexeme];
                    }
                    std::vector<std::string> arg_strs = (*this).resolve_named_args(args, arg_names, fparams, fdefaults, in_method);
                    (*this).apply_extern_wrapping(arg_strs, args, tok.lexeme);
                    return ((((std::string("") + ((*this).resolve_fn_name(tok.lexeme))) + std::string("(")) + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
                }
                std::vector<std::string> arg_strs = (*this).emit_args(args, in_method);
                if ((tok.lexeme == std::string("exit"))) {
                    return ((std::string("lv_exit(") + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
                }
                (*this).apply_extern_wrapping(arg_strs, args, tok.lexeme);
                return ((((std::string("") + ((*this).resolve_fn_name(tok.lexeme))) + std::string("(")) + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
            }
            else {
                std::string func = (*this).emit_expr(callee, in_method);
                std::vector<std::string> arg_strs = (*this).emit_args(args, in_method);
                return ((((std::string("") + (func)) + std::string("(")) + (lv_join(arg_strs, std::string(", ")))) + std::string(")"));
            }
        }
    }

    std::string try_remap_method(const std::string& obj, const std::string& method, const std::vector<std::string>& args) {
        std::string a = lv_join(args, std::string(", "));
        if ((method == std::string("len"))) {
            return ((std::string("static_cast<int64_t>(") + (obj)) + std::string(".size())"));
        }
        else {
            if ((method == std::string("is_empty"))) {
                return ((std::string("") + (obj)) + std::string(".empty()"));
            }
            else {
                if ((method == std::string("contains"))) {
                    return ((((std::string("lv_contains(") + (obj)) + std::string(", ")) + (a)) + std::string(")"));
                }
                else {
                    if ((method == std::string("has"))) {
                        return ((((std::string("(") + (obj)) + std::string(".count(")) + (a)) + std::string(") > 0)"));
                    }
                    else {
                        if ((method == std::string("push"))) {
                            return ((((std::string("") + (obj)) + std::string(".push_back(")) + (a)) + std::string(")"));
                        }
                        else {
                            if ((method == std::string("pop"))) {
                                return ((std::string("lv_pop(") + (obj)) + std::string(")"));
                            }
                            else {
                                if ((method == std::string("add"))) {
                                    return ((((std::string("") + (obj)) + std::string(".insert(")) + (a)) + std::string(")"));
                                }
                                else {
                                    if ((method == std::string("remove"))) {
                                        return ((((std::string("lv_remove(") + (obj)) + std::string(", ")) + (a)) + std::string(")"));
                                    }
                                    else {
                                        if ((method == std::string("clear"))) {
                                            return ((std::string("") + (obj)) + std::string(".clear()"));
                                        }
                                        else {
                                            if ((method == std::string("sort"))) {
                                                return ((std::string("lv_sort(") + (obj)) + std::string(")"));
                                            }
                                            else {
                                                if ((method == std::string("reverse"))) {
                                                    return ((std::string("lv_reverse(") + (obj)) + std::string(")"));
                                                }
                                                else {
                                                    if ((method == std::string("unique"))) {
                                                        return ((std::string("lv_unique(") + (obj)) + std::string(")"));
                                                    }
                                                    else {
                                                        if ((method == std::string("flatten"))) {
                                                            return ((std::string("lv_flatten(") + (obj)) + std::string(")"));
                                                        }
                                                        else {
                                                            if ((method == std::string("slice"))) {
                                                                return ((((std::string("lv_slice(") + (obj)) + std::string(", ")) + (a)) + std::string(")"));
                                                            }
                                                            else {
                                                                if ((method == std::string("keys"))) {
                                                                    return ((std::string("lv_keys(") + (obj)) + std::string(")"));
                                                                }
                                                                else {
                                                                    if ((method == std::string("values"))) {
                                                                        return ((std::string("lv_values(") + (obj)) + std::string(")"));
                                                                    }
                                                                    else {
                                                                        if ((method == std::string("upper"))) {
                                                                            return ((std::string("lv_upper(") + (obj)) + std::string(")"));
                                                                        }
                                                                        else {
                                                                            if ((method == std::string("lower"))) {
                                                                                return ((std::string("lv_lower(") + (obj)) + std::string(")"));
                                                                            }
                                                                            else {
                                                                                if ((method == std::string("trim"))) {
                                                                                    return ((std::string("lv_trim(") + (obj)) + std::string(")"));
                                                                                }
                                                                                else {
                                                                                    if ((method == std::string("pad_left"))) {
                                                                                        return ((((std::string("lv_pad_left(") + (obj)) + std::string(", ")) + (a)) + std::string(")"));
                                                                                    }
                                                                                    else {
                                                                                        if ((method == std::string("pad_right"))) {
                                                                                            return ((((std::string("lv_pad_right(") + (obj)) + std::string(", ")) + (a)) + std::string(")"));
                                                                                        }
                                                                                        else {
                                                                                            if ((method == std::string("repeat"))) {
                                                                                                return ((((std::string("lv_repeat(") + (obj)) + std::string(", ")) + (a)) + std::string(")"));
                                                                                            }
                                                                                            else {
                                                                                                if ((method == std::string("replace"))) {
                                                                                                    return ((((std::string("lv_replace(") + (obj)) + std::string(", ")) + (a)) + std::string(")"));
                                                                                                }
                                                                                                else {
                                                                                                    if ((method == std::string("starts_with"))) {
                                                                                                        return ((((std::string("") + (obj)) + std::string(".starts_with(")) + (a)) + std::string(")"));
                                                                                                    }
                                                                                                    else {
                                                                                                        if ((method == std::string("ends_with"))) {
                                                                                                            return ((((std::string("") + (obj)) + std::string(".ends_with(")) + (a)) + std::string(")"));
                                                                                                        }
                                                                                                        else {
                                                                                                            if ((method == std::string("charAt"))) {
                                                                                                                return ((((std::string("std::string(1, ") + (obj)) + std::string("[")) + (a)) + std::string("])"));
                                                                                                            }
                                                                                                            else {
                                                                                                                if ((method == std::string("join"))) {
                                                                                                                    if ((static_cast<int64_t>(args.size()) > INT64_C(0))) {
                                                                                                                        return ((((std::string("lv_join(") + (obj)) + std::string(", ")) + (args[INT64_C(0)])) + std::string(")"));
                                                                                                                    }
                                                                                                                    return ((std::string("lv_join(") + (obj)) + std::string(", std::string(\"\"))"));
                                                                                                                }
                                                                                                                else {
                                                                                                                    if ((method == std::string("split"))) {
                                                                                                                        if ((static_cast<int64_t>(args.size()) > INT64_C(0))) {
                                                                                                                            return ((((std::string("lv_split(") + (obj)) + std::string(", ")) + (args[INT64_C(0)])) + std::string(")"));
                                                                                                                        }
                                                                                                                        return ((std::string("lv_split(") + (obj)) + std::string(", std::string(\" \"))"));
                                                                                                                    }
                                                                                                                    else {
                                                                                                                        if ((method == std::string("indexOf"))) {
                                                                                                                            if ((static_cast<int64_t>(args.size()) >= INT64_C(2))) {
                                                                                                                                return ((((((std::string("lv_index_of(") + (obj)) + std::string(", ")) + (args[INT64_C(0)])) + std::string(", ")) + (args[INT64_C(1)])) + std::string(")"));
                                                                                                                            }
                                                                                                                            return ((((std::string("lv_index_of(") + (obj)) + std::string(", ")) + (a)) + std::string(")"));
                                                                                                                        }
                                                                                                                        else {
                                                                                                                            if ((method == std::string("substring"))) {
                                                                                                                                if ((static_cast<int64_t>(args.size()) >= INT64_C(2))) {
                                                                                                                                    return ((((((((std::string("") + (obj)) + std::string(".substr(")) + (args[INT64_C(0)])) + std::string(", (")) + (args[INT64_C(1)])) + std::string(") - (")) + (args[INT64_C(0)])) + std::string("))"));
                                                                                                                                }
                                                                                                                                return ((((std::string("") + (obj)) + std::string(".substr(")) + (a)) + std::string(")"));
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return std::string("");
    }

    std::string get_type_category(const Expr& object) {
        {
            const auto& _match_29 = object;
            if (std::holds_alternative<std::decay_t<decltype(_match_29)>::Variable>(_match_29._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_29)>::Variable>(_match_29._data);
                auto& tok = _v.name;
                if ((this->var_types.count(tok.lexeme) > 0)) {
                    TypeNode t = this->var_types[tok.lexeme];
                    return (*this).type_node_to_category(t);
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_29)>::Call>(_match_29._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_29)>::Call>(_match_29._data);
                auto& callee = *_v.callee;
                auto& paren = _v.paren;
                auto& args = _v.args;
                auto& arg_names = _v.arg_names;
                {
                    const auto& _match_30 = callee;
                    if (std::holds_alternative<std::decay_t<decltype(_match_30)>::Get>(_match_30._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_30)>::Get>(_match_30._data);
                        auto& inner_obj = *_v.object;
                        auto& method_name = _v.name;
                        std::string obj_cat = (*this).get_type_category(inner_obj);
                        if ((obj_cat != std::string(""))) {
                            return (*this).infer_method_return_category(obj_cat, method_name.lexeme);
                        }
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else {
                /* pass */
            }
        }
        return std::string("");
    }

    std::string type_node_to_category(const TypeNode& t) {
        {
            const auto& _match_31 = t;
            if (std::holds_alternative<std::decay_t<decltype(_match_31)>::Array>(_match_31._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_31)>::Array>(_match_31._data);
                auto& inner = *_v.inner;
                return std::string("vector");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_31)>::HashMap>(_match_31._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_31)>::HashMap>(_match_31._data);
                auto& k = *_v.key_type;
                auto& v = *_v.value_type;
                return std::string("hashmap");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_31)>::HashSet>(_match_31._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_31)>::HashSet>(_match_31._data);
                auto& inner = *_v.inner;
                return std::string("hashset");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_31)>::Str>(_match_31._data)) {
                return std::string("string");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_31)>::Bytes>(_match_31._data)) {
                return std::string("bytes");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_31)>::Custom>(_match_31._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_31)>::Custom>(_match_31._data);
                auto& name = _v.name;
                auto& type_args = _v.type_args;
                return name;
            }
            else {
                return std::string("");
            }
        }
    }

    std::string infer_method_return_category(const std::string& obj_cat, const std::string& method) {
        if ((obj_cat == std::string("vector"))) {
            if ((method == std::string("map")) || (method == std::string("filter")) || (method == std::string("take")) || (method == std::string("drop"))) {
                return std::string("vector");
            }
            if ((method == std::string("zip")) || (method == std::string("enumerate"))) {
                return std::string("vector");
            }
        }
        if ((obj_cat == std::string("hashset"))) {
            if ((method == std::string("union_with")) || (method == std::string("intersect")) || (method == std::string("difference"))) {
                return std::string("hashset");
            }
        }
        if ((obj_cat == std::string("bytes"))) {
            if ((method == std::string("slice")) || (method == std::string("concat"))) {
                return std::string("bytes");
            }
        }
        if ((obj_cat == std::string("string"))) {
            if ((method == std::string("substring")) || (method == std::string("trim")) || (method == std::string("to_lower")) || (method == std::string("to_upper"))) {
                return std::string("string");
            }
        }
        if ((obj_cat == std::string("bytes"))) {
            if ((method == std::string("to_string")) || (method == std::string("to_hex"))) {
                return std::string("string");
            }
        }
        if ((obj_cat == std::string("string"))) {
            if ((method == std::string("split"))) {
                return std::string("vector");
            }
        }
        if ((obj_cat == std::string("vector"))) {
            if ((method == std::string("join"))) {
                return std::string("string");
            }
        }
        if ((this->extend_methods.count(obj_cat) > 0)) {
            std::vector<Stmt> methods = this->extend_methods[obj_cat];
            for (const auto& ext_m : methods) {
                {
                    const auto& _match_32 = ext_m;
                    if (std::holds_alternative<std::decay_t<decltype(_match_32)>::Function>(_match_32._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_32)>::Function>(_match_32._data);
                        auto& mname = _v.name;
                        auto& mparams = _v.params;
                        auto& mret = _v.return_type;
                        auto& mbody = _v.body;
                        auto& mi = _v.is_inline;
                        auto& mc = _v.comptime_mode;
                        auto& ms = _v.is_static;
                        auto& mv = _v.visibility;
                        auto& mtp = _v.type_params;
                        auto& m_defs = _v.param_defaults;
                        if ((mname.lexeme == method)) {
                            std::string ret_cat = (*this).type_node_to_category(mret);
                            if ((ret_cat != std::string(""))) {
                                return ret_cat;
                            }
                            return obj_cat;
                        }
                    }
                    else {
                        /* pass */
                    }
                }
            }
        }
        return std::string("");
    }

    TypeNode infer_source_type(const Expr& e) {
        {
            const auto& _match_33 = e;
            if (std::holds_alternative<std::decay_t<decltype(_match_33)>::Literal>(_match_33._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_33)>::Literal>(_match_33._data);
                auto& kind = _v.kind;
                auto& value = _v.value;
                if ((kind == std::string("int"))) {
                    return TypeNode::make_Int();
                }
                else {
                    if ((kind == std::string("float"))) {
                        return TypeNode::make_Float();
                    }
                    else {
                        if ((kind == std::string("string"))) {
                            return TypeNode::make_Str();
                        }
                        else {
                            if ((kind == std::string("bool"))) {
                                return TypeNode::make_Bool();
                            }
                        }
                    }
                }
                return TypeNode::make_Auto();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_33)>::Variable>(_match_33._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_33)>::Variable>(_match_33._data);
                auto& tok = _v.name;
                if ((this->var_types.count(tok.lexeme) > 0)) {
                    return this->var_types[tok.lexeme];
                }
                return TypeNode::make_Auto();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_33)>::Grouping>(_match_33._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_33)>::Grouping>(_match_33._data);
                auto& inner = *_v.inner;
                return (*this).infer_source_type(inner);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_33)>::Cast>(_match_33._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_33)>::Cast>(_match_33._data);
                auto& expr = *_v.expr;
                auto& target_type = _v.target_type;
                return target_type;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_33)>::Unary>(_match_33._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_33)>::Unary>(_match_33._data);
                auto& op = _v.op;
                auto& right = *_v.right;
                return (*this).infer_source_type(right);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_33)>::Call>(_match_33._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_33)>::Call>(_match_33._data);
                auto& callee = *_v.callee;
                auto& paren = _v.paren;
                auto& args = _v.args;
                auto& arg_names = _v.arg_names;
                {
                    const auto& _match_34 = callee;
                    if (std::holds_alternative<std::decay_t<decltype(_match_34)>::Get>(_match_34._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_34)>::Get>(_match_34._data);
                        auto& obj = *_v.object;
                        auto& method_tok = _v.name;
                        TypeNode obj_type = (*this).infer_source_type(obj);
                        std::string method = method_tok.lexeme;
                        if ((method == std::string("at"))) {
                            {
                                const auto& _match_35 = obj_type;
                                if (std::holds_alternative<std::decay_t<decltype(_match_35)>::Array>(_match_35._data)) {
                                    auto& _v = std::get<std::decay_t<decltype(_match_35)>::Array>(_match_35._data);
                                    auto& inner = *_v.inner;
                                    return inner;
                                }
                                else if (std::holds_alternative<std::decay_t<decltype(_match_35)>::Custom>(_match_35._data)) {
                                    auto& _v = std::get<std::decay_t<decltype(_match_35)>::Custom>(_match_35._data);
                                    auto& name = _v.name;
                                    auto& targs = _v.type_args;
                                    if ((static_cast<int64_t>(targs.size()) > INT64_C(0))) {
                                        return targs[INT64_C(0)];
                                    }
                                    return TypeNode::make_Auto();
                                }
                                else {
                                    return TypeNode::make_Auto();
                                }
                            }
                        }
                        if ((method == std::string("substring")) || (method == std::string("trim")) || (method == std::string("to_string")) || (method == std::string("join"))) {
                            return TypeNode::make_Str();
                        }
                        if ((method == std::string("len"))) {
                            return TypeNode::make_Int();
                        }
                        return TypeNode::make_Auto();
                    }
                    else {
                        return TypeNode::make_Auto();
                    }
                }
                return TypeNode::make_Auto();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_33)>::Index>(_match_33._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_33)>::Index>(_match_33._data);
                auto& obj = *_v.object;
                auto& bracket = _v.bracket;
                auto& idx = *_v.index;
                TypeNode obj_type = (*this).infer_source_type(obj);
                {
                    const auto& _match_36 = obj_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_36)>::Array>(_match_36._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_36)>::Array>(_match_36._data);
                        auto& inner = *_v.inner;
                        return inner;
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_36)>::Custom>(_match_36._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_36)>::Custom>(_match_36._data);
                        auto& name = _v.name;
                        auto& targs = _v.type_args;
                        if ((static_cast<int64_t>(targs.size()) > INT64_C(0))) {
                            return targs[INT64_C(0)];
                        }
                        return TypeNode::make_Auto();
                    }
                    else {
                        return TypeNode::make_Auto();
                    }
                }
                return TypeNode::make_Auto();
            }
            else {
                return TypeNode::make_Auto();
            }
        }
    }

    std::string try_extend_method(const Expr& object, const std::string& method, const std::string& obj, const std::vector<std::string>& args) {
        std::string type_cat = (*this).get_type_category(object);
        if ((type_cat != std::string("")) && (this->extend_methods.count(type_cat) > 0)) {
            std::vector<Stmt> methods = this->extend_methods[type_cat];
            for (const auto& ext_m : methods) {
                {
                    const auto& _match_37 = ext_m;
                    if (std::holds_alternative<std::decay_t<decltype(_match_37)>::Function>(_match_37._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_37)>::Function>(_match_37._data);
                        auto& mname = _v.name;
                        auto& mparams = _v.params;
                        auto& mret = _v.return_type;
                        auto& mbody = _v.body;
                        auto& is_inline = _v.is_inline;
                        auto& comptime_mode = _v.comptime_mode;
                        auto& is_static = _v.is_static;
                        auto& vis = _v.visibility;
                        auto& tp = _v.type_params;
                        auto& m_defaults = _v.param_defaults;
                        if ((mname.lexeme == method)) {
                            if ((static_cast<int64_t>(args.size()) > INT64_C(0))) {
                                return ((((((((std::string("__ext_") + (type_cat)) + std::string("_")) + (method)) + std::string("(")) + (obj)) + std::string(", ")) + (lv_join(args, std::string(", ")))) + std::string(")"));
                            }
                            return ((((((std::string("__ext_") + (type_cat)) + std::string("_")) + (method)) + std::string("(")) + (obj)) + std::string(")"));
                        }
                    }
                    else {
                        /* pass */
                    }
                }
            }
        }
        return std::string("");
    }

    void emit_extend_method(const std::string& type_key, const Stmt& method) {
        {
            const auto& _match_38 = method;
            if (std::holds_alternative<std::decay_t<decltype(_match_38)>::Function>(_match_38._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_38)>::Function>(_match_38._data);
                auto& mname = _v.name;
                auto& mparams = _v.params;
                auto& mret = _v.return_type;
                auto& mbody = _v.body;
                auto& is_inline = _v.is_inline;
                auto& comptime_mode = _v.comptime_mode;
                auto& is_static = _v.is_static;
                auto& vis = _v.visibility;
                auto& tp = _v.type_params;
                auto& m_defaults = _v.param_defaults;
                std::string ret_type = (*this).emit_type(mret);
                std::string param_str = (*this).emit_params(mparams);
                std::string all_params = std::string("auto&& self");
                if ((param_str != std::string(""))) {
                    all_params = ((std::string("auto&& self, ") + (param_str)) + std::string(""));
                }
                this->output = (this->output + ((((((((((std::string("") + ((*this).indent())) + std::string("")) + (ret_type)) + std::string(" __ext_")) + (type_key)) + std::string("_")) + (mname.lexeme)) + std::string("(")) + (all_params)) + std::string(") {\n")));
                this->indent_level = (this->indent_level + INT64_C(1));
                bool saved_extend = this->in_extend;
                this->in_extend = true;
                for (const auto& st : mbody) {
                    (*this).emit_stmt(st, false);
                }
                this->in_extend = saved_extend;
                this->indent_level = (this->indent_level - INT64_C(1));
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n\n")));
            }
            else {
                /* pass */
            }
        }
    }

    std::string emit_params(const std::vector<Param>& params) {
        std::vector<std::string> strs = {};
        for (const auto& p : params) {
            if (p.is_ref && p.is_mut) {
                strs.push_back(((((std::string("") + ((*this).emit_type(p.param_type))) + std::string("& ")) + (p.name.lexeme)) + std::string("")));
            }
            else {
                if (p.is_ref) {
                    strs.push_back(((((std::string("const ") + ((*this).emit_type(p.param_type))) + std::string("& ")) + (p.name.lexeme)) + std::string("")));
                }
                else {
                    strs.push_back(((((std::string("") + ((*this).emit_type(p.param_type))) + std::string(" ")) + (p.name.lexeme)) + std::string("")));
                }
            }
            this->var_types[p.name.lexeme] = p.param_type;
        }
        return lv_join(strs, std::string(", "));
    }

    std::string emit_lambda(const std::vector<Param>& params, const Expr& body, bool in_method) {
        std::string body_str = (*this).emit_expr(body, in_method);
        return ((((std::string("[&](") + ((*this).emit_params(params))) + std::string(") { return ")) + (body_str)) + std::string("; }"));
    }

    std::string emit_block_lambda(const std::vector<Param>& params, int64_t body_id, bool in_method) {
        std::string param_str = (*this).emit_params(params);
        std::string saved_output = this->output;
        this->output = std::string("");
        int64_t saved_indent = this->indent_level;
        this->indent_level = INT64_C(2);
        std::vector<Stmt> body = this->lambda_blocks[body_id];
        for (const auto& st : body) {
            (*this).emit_stmt(st, in_method);
        }
        std::string body_str = this->output;
        this->output = saved_output;
        this->indent_level = saved_indent;
        return ((((((std::string("[&](") + (param_str)) + std::string(") {\n")) + (body_str)) + std::string("")) + ((*this).indent())) + std::string("    }"));
    }

    void emit_stmt(const Stmt& s, bool m) {
        {
            const auto& _match_39 = s;
            if (_match_39._tag == "None") {
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::ExprStmt>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::ExprStmt>(_match_39._data);
                auto& expr = _v.expr;
                this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("")) + ((*this).emit_expr(expr, m))) + std::string(";\n")));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Let>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Let>(_match_39._data);
                auto& name = _v.name;
                auto& var_type = _v.var_type;
                auto& initializer = _v.initializer;
                auto& visibility = _v.visibility;
                auto& is_ref = _v.is_ref;
                auto& is_mut = _v.is_mut;
                (*this).emit_let(name, var_type, initializer, m, is_ref, is_mut);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Const>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Const>(_match_39._data);
                auto& name = _v.name;
                auto& const_type = _v.const_type;
                auto& value = _v.value;
                auto& visibility = _v.visibility;
                auto& comptime_mode = _v.comptime_mode;
                std::string cpp_type = (*this).emit_type(const_type);
                std::string val = (*this).emit_expr(value, m);
                std::string prefix = std::string("const ");
                if ((comptime_mode == INT64_C(1)) || (comptime_mode == INT64_C(2))) {
                    prefix = std::string("constexpr ");
                }
                this->output = (this->output + ((((((((((std::string("") + ((*this).indent())) + std::string("")) + (prefix)) + std::string("")) + (cpp_type)) + std::string(" ")) + (name.lexeme)) + std::string(" = ")) + (val)) + std::string(";\n")));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Return>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Return>(_match_39._data);
                auto& keyword = _v.keyword;
                auto& value = _v.value;
                {
                    const auto& _match_40 = value;
                    if (_match_40._tag == "None") {
                        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("return;\n")));
                    }
                    else {
                        this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("return ")) + ((*this).emit_expr(value, m))) + std::string(";\n")));
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::If>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::If>(_match_39._data);
                auto& condition = _v.condition;
                auto& then_branch = *_v.then_branch;
                auto& else_branch = *_v.else_branch;
                this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("if (")) + ((*this).emit_expr(condition, m))) + std::string(") ")));
                (*this).emit_block_or_stmt(then_branch, m);
                {
                    const auto& _match_41 = else_branch;
                    if (_match_41._tag == "None") {
                        /* pass */
                    }
                    else {
                        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("else ")));
                        (*this).emit_block_or_stmt(else_branch, m);
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::While>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::While>(_match_39._data);
                auto& condition = _v.condition;
                auto& body = *_v.body;
                this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("while (")) + ((*this).emit_expr(condition, m))) + std::string(") ")));
                (*this).emit_block_or_stmt(body, m);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::For>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::For>(_match_39._data);
                auto& item_name = _v.item_name;
                auto& collection = _v.collection;
                auto& body = *_v.body;
                auto& is_ref = _v.is_ref;
                auto& is_mut = _v.is_mut;
                {
                    const auto& _match_42 = collection;
                    if (std::holds_alternative<std::decay_t<decltype(_match_42)>::Range>(_match_42._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_42)>::Range>(_match_42._data);
                        auto& start = *_v.start;
                        auto& end = *_v.end;
                        this->output = (this->output + ((((((((((((std::string("") + ((*this).indent())) + std::string("for (int64_t ")) + (item_name.lexeme)) + std::string(" = ")) + ((*this).emit_expr(start, m))) + std::string("; ")) + (item_name.lexeme)) + std::string(" < ")) + ((*this).emit_expr(end, m))) + std::string("; ")) + (item_name.lexeme)) + std::string("++) ")));
                        (*this).emit_block_or_stmt(body, m);
                    }
                    else {
                        {
                            const auto& _match_43 = collection;
                            {
                                /* pass */
                            }
                        }
                        std::string for_type = std::string("auto");
                        if (is_ref && is_mut) {
                            for_type = std::string("auto&");
                        }
                        else {
                            if (is_ref) {
                                for_type = std::string("const auto&");
                            }
                        }
                        this->output = (this->output + ((((((((std::string("") + ((*this).indent())) + std::string("for (")) + (for_type)) + std::string(" ")) + (item_name.lexeme)) + std::string(" : ")) + ((*this).emit_expr(collection, m))) + std::string(") ")));
                        (*this).emit_block_or_stmt(body, m);
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Block>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Block>(_match_39._data);
                auto& statements = _v.statements;
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("{\n")));
                this->indent_level = (this->indent_level + INT64_C(1));
                for (const auto& st : statements) {
                    (*this).emit_stmt(st, m);
                }
                this->indent_level = (this->indent_level - INT64_C(1));
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n")));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Try>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Try>(_match_39._data);
                auto& try_body = *_v.try_body;
                auto& catch_body = *_v.catch_body;
                auto& exception_name = _v.exception_name;
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("try ")));
                (*this).emit_block_or_stmt(try_body, m);
                std::string exc_name = std::string("e");
                if ((exception_name != std::string(""))) {
                    exc_name = exception_name;
                }
                this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string(" catch (const std::exception& ")) + (exc_name)) + std::string(") ")));
                (*this).emit_block_or_stmt(catch_body, m);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Function>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Function>(_match_39._data);
                auto& name = _v.name;
                auto& params = _v.params;
                auto& return_type = _v.return_type;
                auto& body = _v.body;
                auto& is_inline = _v.is_inline;
                auto& comptime_mode = _v.comptime_mode;
                auto& is_static = _v.is_static;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                auto& param_defaults = _v.param_defaults;
                (*this).emit_function(name, params, return_type, body, type_params, comptime_mode, param_defaults);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Class>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Class>(_match_39._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                std::vector<std::string> empty_tp = {};
                (*this).emit_class(name, body, empty_tp);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Struct>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Struct>(_match_39._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                (*this).emit_class(name, body, type_params);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Enum>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Enum>(_match_39._data);
                auto& name = _v.name;
                auto& variants = _v.variants;
                auto& methods = _v.methods;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                (*this).emit_enum(name, variants, methods, type_params);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Match>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Match>(_match_39._data);
                auto& expr = _v.expr;
                auto& arm_patterns = _v.arm_patterns;
                auto& arm_bodies = _v.arm_bodies;
                (*this).emit_match_impl(expr, arm_patterns, arm_bodies, m);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Namespace>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Namespace>(_match_39._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Import>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Import>(_match_39._data);
                auto& path = _v.path;
                auto& alias = _v.alias;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Break>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Break>(_match_39._data);
                auto& keyword = _v.keyword;
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("break;\n")));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Continue>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Continue>(_match_39._data);
                auto& keyword = _v.keyword;
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("continue;\n")));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Pass>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Pass>(_match_39._data);
                auto& keyword = _v.keyword;
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("/* pass */\n")));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::CppBlock>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::CppBlock>(_match_39._data);
                auto& code = _v.code;
                auto lines = lv_split(code, std::string("\n"));
                for (const auto& line : lines) {
                    auto trimmed = lv_trim(line);
                    if ((trimmed != std::string(""))) {
                        this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("")) + (trimmed)) + std::string("\n")));
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Extern>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Extern>(_match_39._data);
                auto& header = _v.header;
                auto& import_path = _v.import_path;
                auto& link_lib = _v.link_lib;
                auto& types = _v.types;
                auto& functions = _v.functions;
                std::string inc_line = std::string("");
                bool has_dot = (lv_index_of(header, std::string(".")) >= INT64_C(0));
                if (has_dot) {
                    inc_line = ((std::string("#include \"") + (header)) + std::string("\""));
                }
                else {
                    inc_line = ((std::string("#include <") + (header)) + std::string(">"));
                }
                (*this).push_unique(this->extern_includes, inc_line);
                if ((link_lib != std::string(""))) {
                    (*this).push_unique(this->extern_link_libs, link_lib);
                }
                if ((import_path != std::string(""))) {
                    (*this).push_unique(this->extern_import_paths, import_path);
                }
                for (const auto& et : types) {
                    if ((et.lavina_name != et.cpp_name)) {
                        this->extern_type_names[et.lavina_name] = et.cpp_name;
                    }
                }
                for (const auto& ef : functions) {
                    if ((ef.name != ef.cpp_name)) {
                        this->extern_fn_names[ef.name] = ef.cpp_name;
                    }
                    this->extern_fn_params[ef.name] = ef.params;
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_39)>::Extend>(_match_39._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_39)>::Extend>(_match_39._data);
                auto& target = _v.target_type;
                auto& methods = _v.methods;
                auto& visibility = _v.visibility;
                /* pass */
            }
            else {
                /* pass */
            }
        }
    }

    void emit_let(const Token& name, const TypeNode& var_type, const Expr& initializer, bool in_method, bool is_ref, bool is_mut) {
        std::string cpp_type = (*this).emit_type(var_type);
        this->var_types[name.lexeme] = var_type;
        std::string const_str = std::string("");
        std::string ref_str = std::string("");
        if (is_ref) {
            ref_str = std::string("&");
            if ((!is_mut)) {
                const_str = std::string("const ");
            }
        }
        std::string init_str = std::string("");
        {
            const auto& _match_44 = initializer;
            if (_match_44._tag == "None") {
                init_str = (*this).default_init(cpp_type);
            }
            else {
                std::string val = (*this).emit_expr(initializer, in_method);
                bool is_ptr = false;
                {
                    const auto& _match_45 = var_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_45)>::Ptr>(_match_45._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_45)>::Ptr>(_match_45._data);
                        auto& inner = *_v.inner;
                        is_ptr = true;
                    }
                    else {
                        /* pass */
                    }
                }
                if (is_ptr && (val == std::string("std::nullopt"))) {
                    val = std::string("nullptr");
                }
                init_str = ((std::string(" = ") + (val)) + std::string(""));
            }
        }
        this->output = (this->output + ((((((((((((std::string("") + ((*this).indent())) + std::string("")) + (const_str)) + std::string("")) + (cpp_type)) + std::string("")) + (ref_str)) + std::string(" ")) + (name.lexeme)) + std::string("")) + (init_str)) + std::string(";\n")));
    }

    void emit_block_or_stmt(const Stmt& s, bool m) {
        {
            const auto& _match_46 = s;
            if (std::holds_alternative<std::decay_t<decltype(_match_46)>::Block>(_match_46._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_46)>::Block>(_match_46._data);
                auto& stmts = _v.statements;
                this->output = (this->output + std::string("{\n"));
                this->indent_level = (this->indent_level + INT64_C(1));
                for (const auto& st : stmts) {
                    (*this).emit_stmt(st, m);
                }
                this->indent_level = (this->indent_level - INT64_C(1));
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n")));
            }
            else {
                this->output = (this->output + std::string("{\n"));
                this->indent_level = (this->indent_level + INT64_C(1));
                (*this).emit_stmt(s, m);
                this->indent_level = (this->indent_level - INT64_C(1));
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n")));
            }
        }
    }

    void emit_function(const Token& name, const std::vector<Param>& params, const TypeNode& return_type, const std::vector<Stmt>& body, const std::vector<std::string>& type_params, int64_t comptime_mode, const std::vector<Expr>& defaults) {
        this->fn_params[name.lexeme] = params;
        this->fn_defaults[name.lexeme] = defaults;
        this->output = (this->output + (*this).template_prefix(type_params));
        std::string ret_type = (*this).emit_type(return_type);
        std::string param_str = (*this).emit_params(params);
        std::string prefix = std::string("");
        if ((comptime_mode == INT64_C(1))) {
            prefix = std::string("constexpr ");
        }
        else {
            if ((comptime_mode == INT64_C(2))) {
                prefix = std::string("consteval ");
            }
        }
        this->output = (this->output + ((((((((((std::string("") + ((*this).indent())) + std::string("")) + (prefix)) + std::string("")) + (ret_type)) + std::string(" ")) + (name.lexeme)) + std::string("(")) + (param_str)) + std::string(") {\n")));
        this->indent_level = (this->indent_level + INT64_C(1));
        for (const auto& st : body) {
            (*this).emit_stmt(st, false);
        }
        this->indent_level = (this->indent_level - INT64_C(1));
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n\n")));
    }

    void emit_class_fields(const std::vector<std::string>& init_field_names, const std::vector<std::string>& init_field_types, const std::vector<std::string>& let_field_names, const std::vector<std::string>& let_field_types) {
        for (int64_t fi = INT64_C(0); fi < static_cast<int64_t>(init_field_names.size()); fi++) {
            bool is_let = false;
            for (const auto& lfn : let_field_names) {
                if ((lfn == init_field_names[fi])) {
                    is_let = true;
                }
            }
            if ((!is_let)) {
                this->output = (this->output + ((((((std::string("") + ((*this).indent())) + std::string("")) + (init_field_types[fi])) + std::string(" ")) + (init_field_names[fi])) + std::string(";\n")));
            }
        }
        for (int64_t li = INT64_C(0); li < static_cast<int64_t>(let_field_names.size()); li++) {
            this->output = (this->output + ((((((std::string("") + ((*this).indent())) + std::string("")) + (let_field_types[li])) + std::string(" ")) + (let_field_names[li])) + std::string(";\n")));
        }
        if ((static_cast<int64_t>(init_field_names.size()) > INT64_C(0)) || (static_cast<int64_t>(let_field_names.size()) > INT64_C(0))) {
            this->output = (this->output + std::string("\n"));
        }
    }

    void emit_constructor(const Token& name, const std::vector<Param>& init_params, const std::vector<Stmt>& init_body) {
        std::string param_str = (*this).emit_params(init_params);
        std::vector<std::string> init_list = {};
        std::vector<Stmt> remaining_body = {};
        for (const auto& st : init_body) {
            {
                const auto& _match_47 = st;
                if (std::holds_alternative<std::decay_t<decltype(_match_47)>::ExprStmt>(_match_47._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_47)>::ExprStmt>(_match_47._data);
                    auto& expr = _v.expr;
                    bool handled = false;
                    {
                        const auto& _match_48 = expr;
                        if (std::holds_alternative<std::decay_t<decltype(_match_48)>::Set>(_match_48._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_48)>::Set>(_match_48._data);
                            auto& object = *_v.object;
                            auto& prop = _v.name;
                            auto& value = *_v.value;
                            {
                                const auto& _match_49 = object;
                                if (std::holds_alternative<std::decay_t<decltype(_match_49)>::This>(_match_49._data)) {
                                    auto& _v = std::get<std::decay_t<decltype(_match_49)>::This>(_match_49._data);
                                    auto& kw = _v.keyword;
                                    {
                                        const auto& _match_50 = value;
                                        if (std::holds_alternative<std::decay_t<decltype(_match_50)>::Variable>(_match_50._data)) {
                                            auto& _v = std::get<std::decay_t<decltype(_match_50)>::Variable>(_match_50._data);
                                            auto& tok = _v.name;
                                            init_list.push_back(((((std::string("") + (prop.lexeme)) + std::string("(")) + (tok.lexeme)) + std::string(")")));
                                            handled = true;
                                        }
                                        else {
                                            /* pass */
                                        }
                                    }
                                }
                                else {
                                    /* pass */
                                }
                            }
                        }
                        else {
                            /* pass */
                        }
                    }
                    if ((!handled)) {
                        remaining_body.push_back(st);
                    }
                }
                else {
                    remaining_body.push_back(st);
                }
            }
        }
        if ((static_cast<int64_t>(init_list.size()) == INT64_C(0))) {
            this->output = (this->output + ((((((std::string("") + ((*this).indent())) + std::string("")) + (name.lexeme)) + std::string("(")) + (param_str)) + std::string(") {\n")));
        }
        else {
            this->output = (this->output + ((((((std::string("") + ((*this).indent())) + std::string("")) + (name.lexeme)) + std::string("(")) + (param_str)) + std::string(")\n")));
            this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("    : ")) + (lv_join(init_list, std::string(", ")))) + std::string(" {\n")));
        }
        this->indent_level = (this->indent_level + INT64_C(1));
        for (const auto& st : remaining_body) {
            (*this).emit_stmt(st, true);
        }
        this->indent_level = (this->indent_level - INT64_C(1));
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n\n")));
    }

    void emit_class_method(const Token& class_name, const Stmt& method) {
        {
            const auto& _match_51 = method;
            if (std::holds_alternative<std::decay_t<decltype(_match_51)>::Function>(_match_51._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_51)>::Function>(_match_51._data);
                auto& mname = _v.name;
                auto& mparams = _v.params;
                auto& mret = _v.return_type;
                auto& mbody = _v.body;
                auto& is_inline = _v.is_inline;
                auto& comptime_mode = _v.comptime_mode;
                auto& is_static = _v.is_static;
                auto& visibility = _v.visibility;
                auto& m_tp = _v.type_params;
                auto& m_defs2 = _v.param_defaults;
                std::string ret_type = (*this).emit_type(mret);
                std::string mparam_str = (*this).emit_params(mparams);
                std::string ct_prefix = std::string("");
                if ((comptime_mode == INT64_C(1))) {
                    ct_prefix = std::string("constexpr ");
                }
                else {
                    if ((comptime_mode == INT64_C(2))) {
                        ct_prefix = std::string("consteval ");
                    }
                }
                if ((mname.lexeme == std::string("destructor"))) {
                    this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("~")) + (class_name.lexeme)) + std::string("() {\n")));
                }
                else {
                    if ((mname.lexeme == std::string("to_string"))) {
                        this->output = (this->output + ((((((((((std::string("") + ((*this).indent())) + std::string("")) + (ct_prefix)) + std::string("")) + (ret_type)) + std::string(" ")) + (mname.lexeme)) + std::string("(")) + (mparam_str)) + std::string(") const {\n")));
                    }
                    else {
                        this->output = (this->output + ((((((((((std::string("") + ((*this).indent())) + std::string("")) + (ct_prefix)) + std::string("")) + (ret_type)) + std::string(" ")) + (mname.lexeme)) + std::string("(")) + (mparam_str)) + std::string(") {\n")));
                    }
                }
                this->indent_level = (this->indent_level + INT64_C(1));
                for (const auto& st : mbody) {
                    (*this).emit_stmt(st, true);
                }
                this->indent_level = (this->indent_level - INT64_C(1));
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n\n")));
            }
            else {
                /* pass */
            }
        }
    }

    bool has_method_named(const std::vector<Stmt>& methods, std::string name) {
        for (const auto& m : methods) {
            {
                const auto& _match_52 = m;
                if (std::holds_alternative<std::decay_t<decltype(_match_52)>::Function>(_match_52._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_52)>::Function>(_match_52._data);
                    auto& mname = _v.name;
                    auto& mparams = _v.params;
                    auto& mret = _v.return_type;
                    auto& mbody = _v.body;
                    auto& mi = _v.is_inline;
                    auto& mc = _v.comptime_mode;
                    auto& ms = _v.is_static;
                    auto& mv = _v.visibility;
                    auto& mtp = _v.type_params;
                    auto& m_defs = _v.param_defaults;
                    if ((mname.lexeme == name)) {
                        return true;
                    }
                }
                else {
                    /* pass */
                }
            }
        }
        return false;
    }

    void emit_print_ops(const std::string& full_name, const std::string& tmpl_line, const std::string& to_str_expr) {
        this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("void print(const ")) + (full_name)) + std::string("& _v) { std::cout << ")) + (to_str_expr)) + std::string(" << std::endl; }\n")));
        this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("std::string operator+(const std::string& _s, const ")) + (full_name)) + std::string("& _v) { return _s + ")) + (to_str_expr)) + std::string("; }\n")));
        this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("std::string operator+(const ")) + (full_name)) + std::string("& _v, const std::string& _s) { return ")) + (to_str_expr)) + std::string(" + _s; }\n\n")));
    }

    void emit_class(const Token& name, const std::vector<Stmt>& body, const std::vector<std::string>& type_params) {
        std::vector<Stmt> init_body = {};
        std::vector<Param> init_params = {};
        std::vector<Expr> init_defaults = {};
        bool has_init = false;
        std::vector<Stmt> methods = {};
        std::vector<std::string> let_field_names = {};
        std::vector<std::string> let_field_types = {};
        for (const auto& st : body) {
            {
                const auto& _match_53 = st;
                if (std::holds_alternative<std::decay_t<decltype(_match_53)>::Function>(_match_53._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_53)>::Function>(_match_53._data);
                    auto& fname = _v.name;
                    auto& params = _v.params;
                    auto& return_type = _v.return_type;
                    auto& fbody = _v.body;
                    auto& is_inline = _v.is_inline;
                    auto& comptime_mode = _v.comptime_mode;
                    auto& is_static = _v.is_static;
                    auto& visibility = _v.visibility;
                    auto& fn_tp = _v.type_params;
                    auto& fn_defaults = _v.param_defaults;
                    if ((fname.lexeme == std::string("constructor"))) {
                        has_init = true;
                        init_body = fbody;
                        init_params = params;
                        init_defaults = fn_defaults;
                    }
                    else {
                        methods.push_back(st);
                    }
                }
                else if (std::holds_alternative<std::decay_t<decltype(_match_53)>::Let>(_match_53._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_53)>::Let>(_match_53._data);
                    auto& fname = _v.name;
                    auto& var_type = _v.var_type;
                    auto& init = _v.initializer;
                    auto& visibility = _v.visibility;
                    auto& is_ref = _v.is_ref;
                    auto& is_mut = _v.is_mut;
                    let_field_names.push_back(fname.lexeme);
                    let_field_types.push_back((*this).emit_type(var_type));
                }
                else {
                    /* pass */
                }
            }
        }
        std::vector<std::string> init_field_names = {};
        std::vector<std::string> init_field_types = {};
        if (has_init) {
            std::vector<std::string> param_names = {};
            std::vector<std::string> param_types = {};
            for (const auto& p : init_params) {
                param_names.push_back(p.name.lexeme);
                param_types.push_back((*this).emit_type(p.param_type));
            }
            std::vector<std::string> seen = {};
            for (const auto& st : init_body) {
                {
                    const auto& _match_54 = st;
                    if (std::holds_alternative<std::decay_t<decltype(_match_54)>::ExprStmt>(_match_54._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_54)>::ExprStmt>(_match_54._data);
                        auto& expr = _v.expr;
                        {
                            const auto& _match_55 = expr;
                            if (std::holds_alternative<std::decay_t<decltype(_match_55)>::Set>(_match_55._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_55)>::Set>(_match_55._data);
                                auto& object = *_v.object;
                                auto& prop = _v.name;
                                auto& value = *_v.value;
                                {
                                    const auto& _match_56 = object;
                                    if (std::holds_alternative<std::decay_t<decltype(_match_56)>::This>(_match_56._data)) {
                                        auto& _v = std::get<std::decay_t<decltype(_match_56)>::This>(_match_56._data);
                                        auto& kw = _v.keyword;
                                        bool already = false;
                                        for (const auto& s : seen) {
                                            if ((s == prop.lexeme)) {
                                                already = true;
                                            }
                                        }
                                        if ((!already)) {
                                            seen.push_back(prop.lexeme);
                                            std::string cpp_type = (*this).infer_expr_type(value, param_names, param_types);
                                            init_field_names.push_back(prop.lexeme);
                                            init_field_types.push_back(cpp_type);
                                        }
                                    }
                                    else {
                                        /* pass */
                                    }
                                }
                            }
                            else {
                                /* pass */
                            }
                        }
                    }
                    else {
                        /* pass */
                    }
                }
            }
        }
        this->output = (this->output + ((((((std::string("") + ((*this).template_prefix(type_params))) + std::string("")) + ((*this).indent())) + std::string("struct ")) + (name.lexeme)) + std::string(" {\n")));
        this->indent_level = (this->indent_level + INT64_C(1));
        (*this).emit_class_fields(init_field_names, init_field_types, let_field_names, let_field_types);
        if (has_init) {
            this->fn_params[name.lexeme] = init_params;
            this->fn_defaults[name.lexeme] = init_defaults;
            (*this).emit_constructor(name, init_params, init_body);
        }
        for (const auto& m : methods) {
            (*this).emit_class_method(name, m);
        }
        this->indent_level = (this->indent_level - INT64_C(1));
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("};\n\n")));
        if ((*this).has_method_named(methods, std::string("to_string"))) {
            std::string tmpl_line = (*this).template_prefix(type_params);
            std::string type_suffix = std::string("");
            if ((static_cast<int64_t>(type_params.size()) > INT64_C(0))) {
                type_suffix = ((std::string("<") + (lv_join(type_params, std::string(", ")))) + std::string(">"));
            }
            (*this).emit_print_ops(((((std::string("") + (name.lexeme)) + std::string("")) + (type_suffix)) + std::string("")), tmpl_line, std::string("_v.to_string()"));
        }
    }

    std::string infer_expr_type(const Expr& e, const std::vector<std::string>& param_names, const std::vector<std::string>& param_types) {
        {
            const auto& _match_57 = e;
            if (std::holds_alternative<std::decay_t<decltype(_match_57)>::Literal>(_match_57._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_57)>::Literal>(_match_57._data);
                auto& kind = _v.kind;
                auto& value = _v.value;
                if ((kind == std::string("int"))) {
                    return std::string("int64_t");
                }
                else {
                    if ((kind == std::string("float"))) {
                        return std::string("double");
                    }
                    else {
                        if ((kind == std::string("string"))) {
                            return std::string("std::string");
                        }
                        else {
                            if ((kind == std::string("bool"))) {
                                return std::string("bool");
                            }
                            else {
                                if ((kind == std::string("null"))) {
                                    return std::string("std::nullptr_t");
                                }
                            }
                        }
                    }
                }
                return std::string("std::any");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_57)>::Variable>(_match_57._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_57)>::Variable>(_match_57._data);
                auto& tok = _v.name;
                for (int64_t i = INT64_C(0); i < static_cast<int64_t>(param_names.size()); i++) {
                    if ((param_names[i] == tok.lexeme)) {
                        if ((param_types[i] == std::string("auto"))) {
                            return std::string("std::any");
                        }
                        return param_types[i];
                    }
                }
                return std::string("std::any");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_57)>::Binary>(_match_57._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_57)>::Binary>(_match_57._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                std::string lt = (*this).infer_expr_type(left, param_names, param_types);
                std::string rt = (*this).infer_expr_type(right, param_names, param_types);
                if ((lt == rt) && (lt != std::string("std::any"))) {
                    return lt;
                }
                if ((lt == std::string("std::string")) || (rt == std::string("std::string"))) {
                    return std::string("std::string");
                }
                return std::string("std::any");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_57)>::Unary>(_match_57._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_57)>::Unary>(_match_57._data);
                auto& op = _v.op;
                auto& right = *_v.right;
                return (*this).infer_expr_type(right, param_names, param_types);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_57)>::Grouping>(_match_57._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_57)>::Grouping>(_match_57._data);
                auto& inner = *_v.inner;
                return (*this).infer_expr_type(inner, param_names, param_types);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_57)>::Call>(_match_57._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_57)>::Call>(_match_57._data);
                auto& callee = *_v.callee;
                auto& paren = _v.paren;
                auto& args = _v.args;
                auto& arg_names = _v.arg_names;
                {
                    const auto& _match_58 = callee;
                    if (std::holds_alternative<std::decay_t<decltype(_match_58)>::Variable>(_match_58._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_58)>::Variable>(_match_58._data);
                        auto& tok = _v.name;
                        if ((*this).is_known_enum(tok.lexeme)) {
                            return tok.lexeme;
                        }
                        for (int64_t ci = INT64_C(0); ci < static_cast<int64_t>(param_names.size()); ci++) {
                            if ((param_names[ci] == tok.lexeme)) {
                                return param_types[ci];
                            }
                        }
                        return tok.lexeme;
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_58)>::StaticGet>(_match_58._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_58)>::StaticGet>(_match_58._data);
                        auto& object = *_v.object;
                        auto& name = _v.name;
                        {
                            const auto& _match_59 = object;
                            if (std::holds_alternative<std::decay_t<decltype(_match_59)>::Variable>(_match_59._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_59)>::Variable>(_match_59._data);
                                auto& tok = _v.name;
                                if ((*this).is_known_enum(tok.lexeme)) {
                                    return tok.lexeme;
                                }
                            }
                            else {
                                /* pass */
                            }
                        }
                        return std::string("std::any");
                    }
                    else {
                        return std::string("std::any");
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_57)>::Vector>(_match_57._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_57)>::Vector>(_match_57._data);
                auto& elements = _v.elements;
                if ((static_cast<int64_t>(elements.size()) > INT64_C(0))) {
                    std::string inner = (*this).infer_expr_type(elements[INT64_C(0)], param_names, param_types);
                    if ((inner != std::string("std::any"))) {
                        return ((std::string("std::vector<") + (inner)) + std::string(">"));
                    }
                }
                return std::string("std::vector<std::any>");
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_57)>::Map>(_match_57._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_57)>::Map>(_match_57._data);
                auto& keys = _v.keys;
                auto& values = _v.values;
                std::string kt = std::string("std::any");
                std::string vt = std::string("std::any");
                if ((static_cast<int64_t>(keys.size()) > INT64_C(0))) {
                    kt = (*this).infer_expr_type(keys[INT64_C(0)], param_names, param_types);
                    vt = (*this).infer_expr_type(values[INT64_C(0)], param_names, param_types);
                }
                return ((((std::string("std::unordered_map<") + (kt)) + std::string(", ")) + (vt)) + std::string(">"));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_57)>::Cast>(_match_57._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_57)>::Cast>(_match_57._data);
                auto& expr = *_v.expr;
                auto& target_type = _v.target_type;
                return (*this).emit_type(target_type);
            }
            else {
                return std::string("std::any");
            }
        }
    }

    void emit_enum_structs(const std::string& enum_name, const std::vector<EnumVariantNode>& variants, const std::string& type_suffix) {
        for (const auto& v : variants) {
            if ((static_cast<int64_t>(v.types.size()) > INT64_C(0))) {
                this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("struct ")) + (v.name.lexeme)) + std::string(" { ")));
                for (int64_t fi = INT64_C(0); fi < static_cast<int64_t>(v.types.size()); fi++) {
                    std::string cpp_type = (*this).emit_type(v.types[fi]);
                    std::string fname = v.field_names[fi];
                    {
                        const auto& _match_60 = v.types[fi];
                        if (std::holds_alternative<std::decay_t<decltype(_match_60)>::Custom>(_match_60._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_60)>::Custom>(_match_60._data);
                            auto& n = _v.name;
                            auto& _ = _v.type_args;
                            if ((n == enum_name)) {
                                cpp_type = ((((std::string("std::shared_ptr<") + (enum_name)) + std::string("")) + (type_suffix)) + std::string(">"));
                            }
                        }
                        else {
                            /* pass */
                        }
                    }
                    this->output = (this->output + ((((std::string("") + (cpp_type)) + std::string(" ")) + (fname)) + std::string("; ")));
                }
                this->output = (this->output + std::string("};\n"));
            }
            else {
                this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("struct ")) + (v.name.lexeme)) + std::string(" {};\n")));
            }
        }
        this->output = (this->output + std::string("\n"));
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("std::string _tag;\n")));
        std::vector<std::string> variant_inner_types = {};
        for (const auto& v : variants) {
            variant_inner_types.push_back(((((std::string("") + (enum_name)) + std::string("::")) + (v.name.lexeme)) + std::string("")));
        }
        this->output = (this->output + ((((std::string("") + ((*this).indent())) + std::string("std::variant<")) + (lv_join(variant_inner_types, std::string(", ")))) + std::string("> _data;\n")));
        this->output = (this->output + std::string("\n"));
    }

    void emit_enum_makers(const std::string& enum_name, const std::vector<EnumVariantNode>& variants, const std::string& type_suffix) {
        for (const auto& v : variants) {
            std::string vname = v.name.lexeme;
            if ((static_cast<int64_t>(v.types.size()) == INT64_C(0))) {
                this->output = (this->output + ((((((((((std::string("") + ((*this).indent())) + std::string("static ")) + (enum_name)) + std::string(" make_")) + (vname)) + std::string("() { return {\"")) + (vname)) + std::string("\", ")) + (vname)) + std::string("{}}; }\n")));
            }
            else {
                std::vector<std::string> params = {};
                std::vector<std::string> field_inits = {};
                for (int64_t fi = INT64_C(0); fi < static_cast<int64_t>(v.types.size()); fi++) {
                    std::string cpp_type = (*this).emit_type(v.types[fi]);
                    std::string fname = v.field_names[fi];
                    bool is_self_ref = false;
                    {
                        const auto& _match_61 = v.types[fi];
                        if (std::holds_alternative<std::decay_t<decltype(_match_61)>::Custom>(_match_61._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_61)>::Custom>(_match_61._data);
                            auto& n = _v.name;
                            auto& _ = _v.type_args;
                            if ((n == enum_name)) {
                                is_self_ref = true;
                            }
                        }
                        else {
                            /* pass */
                        }
                    }
                    if (is_self_ref) {
                        params.push_back(((((((std::string("") + (enum_name)) + std::string("")) + (type_suffix)) + std::string(" ")) + (fname)) + std::string("")));
                        field_inits.push_back(((((((std::string("std::make_shared<") + (enum_name)) + std::string("")) + (type_suffix)) + std::string(">(std::move(")) + (fname)) + std::string("))")));
                    }
                    else {
                        params.push_back(((((std::string("") + (cpp_type)) + std::string(" ")) + (fname)) + std::string("")));
                        field_inits.push_back(fname);
                    }
                }
                this->output = (this->output + ((((((((((((((std::string("") + ((*this).indent())) + std::string("static ")) + (enum_name)) + std::string(" make_")) + (vname)) + std::string("(")) + (lv_join(params, std::string(", ")))) + std::string(") { return {\"")) + (vname)) + std::string("\", ")) + (vname)) + std::string("{")) + (lv_join(field_inits, std::string(", ")))) + std::string("}}; }\n")));
            }
        }
        this->output = (this->output + std::string("\n"));
    }

    void emit_enum_operators(const std::string& enum_name, const std::string& type_suffix, const std::string& tmpl_line, bool has_to_string) {
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("std::string operator[](const std::string& key) const {\n")));
        this->indent_level = (this->indent_level + INT64_C(1));
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("if (key == \"_tag\") return _tag;\n")));
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("return \"\";\n")));
        this->indent_level = (this->indent_level - INT64_C(1));
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n")));
        this->indent_level = (this->indent_level - INT64_C(1));
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("};\n\n")));
        std::string full_type = ((((std::string("") + (enum_name)) + std::string("")) + (type_suffix)) + std::string(""));
        if (has_to_string) {
            (*this).emit_print_ops(full_type, tmpl_line, std::string("_v.to_string()"));
        }
        else {
            this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("void print(const ")) + (full_type)) + std::string("& _v) { std::cout << \"")) + (enum_name)) + std::string("(\" << _v._tag << \")\" << std::endl; }\n")));
            this->output = (this->output + ((((std::string("") + (tmpl_line)) + std::string("std::string operator+(const std::string& _s, const ")) + (full_type)) + std::string("& _v) { return _s + _v._tag; }\n")));
            this->output = (this->output + ((((std::string("") + (tmpl_line)) + std::string("std::string operator+(const ")) + (full_type)) + std::string("& _v, const std::string& _s) { return _v._tag + _s; }\n\n")));
        }
    }

    void emit_enum(const Token& name, const std::vector<EnumVariantNode>& variants, const std::vector<Stmt>& methods, const std::vector<std::string>& type_params) {
        std::string enum_name = name.lexeme;
        this->known_enums[enum_name] = variants;
        bool has_self_ref = false;
        for (const auto& v : variants) {
            for (const auto& ft : v.types) {
                {
                    const auto& _match_62 = ft;
                    if (std::holds_alternative<std::decay_t<decltype(_match_62)>::Custom>(_match_62._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_62)>::Custom>(_match_62._data);
                        auto& n = _v.name;
                        auto& _ = _v.type_args;
                        if ((n == enum_name)) {
                            has_self_ref = true;
                        }
                    }
                    else {
                        /* pass */
                    }
                }
            }
        }
        std::string tmpl_line = (*this).template_prefix(type_params);
        std::string type_suffix = std::string("");
        if ((static_cast<int64_t>(type_params.size()) > INT64_C(0))) {
            type_suffix = ((std::string("<") + (lv_join(type_params, std::string(", ")))) + std::string(">"));
        }
        if (has_self_ref) {
            this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("")) + ((*this).indent())) + std::string("struct ")) + (enum_name)) + std::string(";\n")));
        }
        this->output = (this->output + ((((((std::string("") + (tmpl_line)) + std::string("")) + ((*this).indent())) + std::string("struct ")) + (enum_name)) + std::string(" {\n")));
        this->indent_level = (this->indent_level + INT64_C(1));
        (*this).emit_enum_structs(enum_name, variants, type_suffix);
        (*this).emit_enum_makers(enum_name, variants, type_suffix);
        for (const auto& m : methods) {
            (*this).emit_class_method(name, m);
        }
        bool has_to_string = (*this).has_method_named(methods, std::string("to_string"));
        (*this).emit_enum_operators(enum_name, type_suffix, tmpl_line, has_to_string);
    }

    void emit_match_impl(const Expr& expr, const std::vector<MatchArm>& arm_patterns, const std::vector<Stmt>& arm_bodies, bool in_method) {
        std::string match_val = (*this).emit_expr(expr, in_method);
        std::string temp = ((std::string("_match_") + (this->temp_counter)) + std::string(""));
        this->temp_counter = (this->temp_counter + INT64_C(1));
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("{\n")));
        this->indent_level = (this->indent_level + INT64_C(1));
        this->output = (this->output + ((((((std::string("") + ((*this).indent())) + std::string("const auto& ")) + (temp)) + std::string(" = ")) + (match_val)) + std::string(";\n")));
        bool first = true;
        for (int64_t ai = INT64_C(0); ai < static_cast<int64_t>(arm_patterns.size()); ai++) {
            auto arm = arm_patterns[ai];
            Stmt arm_body = arm_bodies[ai];
            if ((arm.pattern_name == std::string("_"))) {
                if (first) {
                    this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("{\n")));
                }
                else {
                    this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("else {\n")));
                }
                this->indent_level = (this->indent_level + INT64_C(1));
                (*this).emit_arm_body(arm_body, in_method);
                this->indent_level = (this->indent_level - INT64_C(1));
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n")));
            }
            else {
                std::string keyword = std::string("if");
                if ((!first)) {
                    keyword = std::string("else if");
                }
                first = false;
                std::string ename = find_enum_for_variant(this->known_enums, arm.pattern_name);
                if ((ename != std::string(""))) {
                    this->output = (this->output + ((((((((((std::string("") + ((*this).indent())) + std::string("")) + (keyword)) + std::string(" (std::holds_alternative<std::decay_t<decltype(")) + (temp)) + std::string(")>::")) + (arm.pattern_name)) + std::string(">(")) + (temp)) + std::string("._data)) {\n")));
                }
                else {
                    this->output = (this->output + ((((((((std::string("") + ((*this).indent())) + std::string("")) + (keyword)) + std::string(" (")) + (temp)) + std::string("._tag == \"")) + (arm.pattern_name)) + std::string("\") {\n")));
                }
                this->indent_level = (this->indent_level + INT64_C(1));
                if ((static_cast<int64_t>(arm.bindings.size()) > INT64_C(0))) {
                    if ((ename != std::string(""))) {
                        this->output = (this->output + ((((((((std::string("") + ((*this).indent())) + std::string("auto& _v = std::get<std::decay_t<decltype(")) + (temp)) + std::string(")>::")) + (arm.pattern_name)) + std::string(">(")) + (temp)) + std::string("._data);\n")));
                        auto vinfo = (*this).get_variant_info(ename, arm.pattern_name);
                        for (int64_t bi = INT64_C(0); bi < static_cast<int64_t>(arm.bindings.size()); bi++) {
                            if ((bi < static_cast<int64_t>(vinfo.field_names.size()))) {
                                std::string field_name = vinfo.field_names[bi];
                                bool is_self_ref = false;
                                if ((bi < static_cast<int64_t>(vinfo.types.size()))) {
                                    {
                                        const auto& _match_63 = vinfo.types[bi];
                                        if (std::holds_alternative<std::decay_t<decltype(_match_63)>::Custom>(_match_63._data)) {
                                            auto& _v = std::get<std::decay_t<decltype(_match_63)>::Custom>(_match_63._data);
                                            auto& n = _v.name;
                                            auto& _ = _v.type_args;
                                            if ((n == ename)) {
                                                is_self_ref = true;
                                            }
                                        }
                                        else {
                                            /* pass */
                                        }
                                    }
                                }
                                if (is_self_ref) {
                                    this->output = (this->output + ((((((std::string("") + ((*this).indent())) + std::string("auto& ")) + (arm.bindings[bi])) + std::string(" = *_v.")) + (field_name)) + std::string(";\n")));
                                }
                                else {
                                    this->output = (this->output + ((((((std::string("") + ((*this).indent())) + std::string("auto& ")) + (arm.bindings[bi])) + std::string(" = _v.")) + (field_name)) + std::string(";\n")));
                                }
                            }
                        }
                    }
                }
                (*this).emit_arm_body(arm_body, in_method);
                this->indent_level = (this->indent_level - INT64_C(1));
                this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n")));
            }
        }
        this->indent_level = (this->indent_level - INT64_C(1));
        this->output = (this->output + ((std::string("") + ((*this).indent())) + std::string("}\n")));
    }

    void emit_arm_body(const Stmt& arm_body, bool in_method) {
        {
            const auto& _match_64 = arm_body;
            if (std::holds_alternative<std::decay_t<decltype(_match_64)>::Block>(_match_64._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_64)>::Block>(_match_64._data);
                auto& stmts = _v.statements;
                for (const auto& st : stmts) {
                    (*this).emit_stmt(st, in_method);
                }
            }
            else {
                (*this).emit_stmt(arm_body, in_method);
            }
        }
    }

    void emit_using_if_public(const std::string& ns, const Stmt& stmt) {
        {
            const auto& _match_65 = stmt;
            if (std::holds_alternative<std::decay_t<decltype(_match_65)>::Function>(_match_65._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_65)>::Function>(_match_65._data);
                auto& name = _v.name;
                auto& params = _v.params;
                auto& return_type = _v.return_type;
                auto& body = _v.body;
                auto& is_inline = _v.is_inline;
                auto& comptime_mode = _v.comptime_mode;
                auto& is_static = _v.is_static;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                auto& param_defaults = _v.param_defaults;
                if ((visibility != std::string("private"))) {
                    this->output = (this->output + ((((std::string("using ") + (ns)) + std::string("::")) + (name.lexeme)) + std::string(";\n")));
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_65)>::Class>(_match_65._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_65)>::Class>(_match_65._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                if ((visibility != std::string("private"))) {
                    this->output = (this->output + ((((std::string("using ") + (ns)) + std::string("::")) + (name.lexeme)) + std::string(";\n")));
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_65)>::Struct>(_match_65._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_65)>::Struct>(_match_65._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                if ((visibility != std::string("private"))) {
                    this->output = (this->output + ((((std::string("using ") + (ns)) + std::string("::")) + (name.lexeme)) + std::string(";\n")));
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_65)>::Enum>(_match_65._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_65)>::Enum>(_match_65._data);
                auto& name = _v.name;
                auto& variants = _v.variants;
                auto& methods = _v.methods;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                if ((visibility != std::string("private"))) {
                    this->output = (this->output + ((((std::string("using ") + (ns)) + std::string("::")) + (name.lexeme)) + std::string(";\n")));
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_65)>::Const>(_match_65._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_65)>::Const>(_match_65._data);
                auto& name = _v.name;
                auto& const_type = _v.const_type;
                auto& value = _v.value;
                auto& visibility = _v.visibility;
                auto& comptime_mode = _v.comptime_mode;
                if ((visibility != std::string("private"))) {
                    this->output = (this->output + ((((std::string("using ") + (ns)) + std::string("::")) + (name.lexeme)) + std::string(";\n")));
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_65)>::Let>(_match_65._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_65)>::Let>(_match_65._data);
                auto& name = _v.name;
                auto& var_type = _v.var_type;
                auto& initializer = _v.initializer;
                auto& visibility = _v.visibility;
                auto& is_ref = _v.is_ref;
                auto& is_mut = _v.is_mut;
                if ((visibility != std::string("private"))) {
                    this->output = (this->output + ((((std::string("using ") + (ns)) + std::string("::")) + (name.lexeme)) + std::string(";\n")));
                }
            }
            else {
                /* pass */
            }
        }
    }

    void emit_module(int64_t index) {
        std::string full_name = this->module_full_names[index];
        std::string short_name = this->module_short_names[index];
        std::string alias = this->module_aliases[index];
        this->output = (this->output + ((std::string("namespace ") + (full_name)) + std::string(" {\n")));
        this->indent_level = INT64_C(1);
        for (const auto& stmt : this->module_stmts[index]) {
            {
                const auto& _match_66 = stmt;
                if (std::holds_alternative<std::decay_t<decltype(_match_66)>::Function>(_match_66._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_66)>::Function>(_match_66._data);
                    auto& name = _v.name;
                    auto& params = _v.params;
                    auto& return_type = _v.return_type;
                    auto& body = _v.body;
                    auto& is_inline = _v.is_inline;
                    auto& comptime_mode = _v.comptime_mode;
                    auto& is_static = _v.is_static;
                    auto& visibility = _v.visibility;
                    auto& type_params = _v.type_params;
                    auto& param_defaults = _v.param_defaults;
                    if ((name.lexeme == std::string("main"))) {
                        /* pass */
                    }
                    else {
                        (*this).emit_stmt(stmt, false);
                    }
                }
                else if (std::holds_alternative<std::decay_t<decltype(_match_66)>::Extern>(_match_66._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_66)>::Extern>(_match_66._data);
                    auto& header = _v.header;
                    auto& import_path = _v.import_path;
                    auto& link_lib = _v.link_lib;
                    auto& types = _v.types;
                    auto& functions = _v.functions;
                    /* pass */
                }
                else {
                    (*this).emit_stmt(stmt, false);
                }
            }
        }
        this->indent_level = INT64_C(0);
        this->output = (this->output + ((std::string("} // namespace ") + (full_name)) + std::string("\n")));
        if ((short_name != full_name)) {
            this->output = (this->output + ((((std::string("namespace ") + (short_name)) + std::string(" = ")) + (full_name)) + std::string(";\n")));
        }
        if ((alias != std::string(""))) {
            this->output = (this->output + ((((std::string("namespace ") + (alias)) + std::string(" = ")) + (full_name)) + std::string(";\n")));
        }
        if ((!full_name.starts_with(std::string("std_")))) {
            for (const auto& stmt : this->module_stmts[index]) {
                (*this).emit_using_if_public(full_name, stmt);
            }
        }
        this->output = (this->output + std::string("\n"));
        this->declarations = (this->declarations + this->output);
        this->output = std::string("");
    }

    void collect_externs(const std::vector<Stmt>& stmts) {
        for (const auto& stmt : stmts) {
            {
                const auto& _match_67 = stmt;
                if (std::holds_alternative<std::decay_t<decltype(_match_67)>::Extern>(_match_67._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_67)>::Extern>(_match_67._data);
                    auto& header = _v.header;
                    auto& import_path = _v.import_path;
                    auto& link_lib = _v.link_lib;
                    auto& types = _v.types;
                    auto& functions = _v.functions;
                    (*this).emit_stmt(stmt, false);
                    this->output = std::string("");
                }
                else {
                    /* pass */
                }
            }
        }
    }

    void collect_extends(const std::vector<Stmt>& stmts) {
        for (const auto& stmt : stmts) {
            {
                const auto& _match_68 = stmt;
                if (std::holds_alternative<std::decay_t<decltype(_match_68)>::Extend>(_match_68._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_68)>::Extend>(_match_68._data);
                    auto& target = _v.target_type;
                    auto& methods = _v.methods;
                    auto& visibility = _v.visibility;
                    std::string key = target.lexeme;
                    if ((!(this->extend_methods.count(key) > 0))) {
                        std::vector<Stmt> empty = {};
                        this->extend_methods[key] = empty;
                    }
                    for (const auto& m : methods) {
                        this->extend_methods[key].push_back(m);
                    }
                }
                else {
                    /* pass */
                }
            }
        }
    }

    void emit_main_stmts(const std::vector<Stmt>& stmts) {
        for (const auto& stmt : stmts) {
            {
                const auto& _match_69 = stmt;
                if (std::holds_alternative<std::decay_t<decltype(_match_69)>::Function>(_match_69._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_69)>::Function>(_match_69._data);
                    auto& name = _v.name;
                    auto& params = _v.params;
                    auto& return_type = _v.return_type;
                    auto& body = _v.body;
                    auto& is_inline = _v.is_inline;
                    auto& comptime_mode = _v.comptime_mode;
                    auto& is_static = _v.is_static;
                    auto& visibility = _v.visibility;
                    auto& type_params = _v.type_params;
                    auto& param_defaults = _v.param_defaults;
                    if ((name.lexeme == std::string("main"))) {
                        this->has_main = true;
                        this->output = (this->output + std::string("int main(int argc, char* argv[]) {\n"));
                        this->output = (this->output + std::string("    for (int i = 0; i < argc; i++) _lv_args.push_back(argv[i]);\n"));
                        this->indent_level = INT64_C(1);
                        for (const auto& s : body) {
                            (*this).emit_stmt(s, false);
                        }
                        this->indent_level = INT64_C(0);
                        this->output = (this->output + std::string("}\n"));
                        this->declarations = (this->declarations + this->output);
                        this->output = std::string("");
                    }
                    else {
                        (*this).emit_stmt(stmt, false);
                        this->declarations = (this->declarations + this->output);
                        this->output = std::string("");
                    }
                }
                else if (std::holds_alternative<std::decay_t<decltype(_match_69)>::Extern>(_match_69._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_69)>::Extern>(_match_69._data);
                    auto& header = _v.header;
                    auto& import_path = _v.import_path;
                    auto& link_lib = _v.link_lib;
                    auto& types = _v.types;
                    auto& functions = _v.functions;
                    /* pass */
                }
                else if (std::holds_alternative<std::decay_t<decltype(_match_69)>::Extend>(_match_69._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_69)>::Extend>(_match_69._data);
                    auto& target = _v.target_type;
                    auto& methods = _v.methods;
                    auto& visibility = _v.visibility;
                    /* pass */
                }
                else {
                    (*this).emit_stmt(stmt, false);
                    this->declarations = (this->declarations + this->output);
                    this->output = std::string("");
                }
            }
        }
    }

    std::string generate(const std::vector<Stmt>& stmts) {
        for (int64_t mi = INT64_C(0); mi < static_cast<int64_t>(this->module_stmts.size()); mi++) {
            (*this).collect_externs(this->module_stmts[mi]);
        }
        (*this).collect_externs(stmts);
        for (int64_t mi = INT64_C(0); mi < static_cast<int64_t>(this->module_stmts.size()); mi++) {
            (*this).collect_extends(this->module_stmts[mi]);
        }
        (*this).collect_extends(stmts);
        this->declarations = std::string("#include \"lavina.h\"\n");
        for (const auto& inc : this->extern_includes) {
            this->declarations = (this->declarations + ((std::string("") + (inc)) + std::string("\n")));
        }
        this->declarations = (this->declarations + std::string("\n"));
        for (int64_t mi = INT64_C(0); mi < static_cast<int64_t>(this->module_stmts.size()); mi++) {
            (*this).emit_module(mi);
        }
        auto ext_keys = lv_keys(this->extend_methods);
        for (const auto& ek : ext_keys) {
            std::vector<Stmt> methods = this->extend_methods[ek];
            for (const auto& m : methods) {
                (*this).emit_extend_method(ek, m);
            }
            this->declarations = (this->declarations + this->output);
            this->output = std::string("");
        }
        (*this).emit_main_stmts(stmts);
        return this->declarations;
    }

};

struct Symbol {
    std::string name;
    TypeNode sym_type;
    std::string kind;
    bool is_ref;
    bool is_mutable;

};

struct Scope {
    std::vector<std::string> names;
    std::vector<Symbol> symbols;

};

struct Checker {
    std::vector<Scope> scopes;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    std::unordered_map<std::string, std::vector<EnumVariantNode>> known_enums;
    std::unordered_map<std::string, ExternFn> known_funcs;
    std::unordered_map<std::string, std::vector<Stmt>> known_classes;
    TypeNode current_return_type;
    bool in_class;
    std::string current_class_name;

    Checker() {
        this->scopes = {};
        this->errors = {};
        this->warnings = {};
        this->known_enums = {{}};
        this->known_funcs = {{}};
        this->known_classes = {{}};
        this->current_return_type = TypeNode::make_None();
        this->in_class = false;
        this->current_class_name = std::string("");
    }

    void error(const std::string& msg, const Token& t) {
        this->errors.push_back(((((((std::string("[") + (t.line)) + std::string(":")) + (t.col)) + std::string("] Error: ")) + (msg)) + std::string("")));
    }

    void warn(const std::string& msg, const Token& t) {
        this->warnings.push_back(((((((std::string("[") + (t.line)) + std::string(":")) + (t.col)) + std::string("] Warning: ")) + (msg)) + std::string("")));
    }

    void push_scope() {
        std::vector<std::string> names = {};
        std::vector<Symbol> syms = {};
        this->scopes.push_back(Scope(names, syms));
    }

    void pop_scope() {
        lv_pop(this->scopes);
    }

    void declare(const std::string& name, const TypeNode& t, const std::string& kind, bool is_ref, bool is_mutable, const Token& tok) {
        if ((static_cast<int64_t>(this->scopes.size()) == INT64_C(0))) {
            return;
        }
        auto& scope = this->scopes[(static_cast<int64_t>(this->scopes.size()) - INT64_C(1))];
        for (const auto& n : scope.names) {
            if ((n == name)) {
                (*this).warn(((std::string("Redefinition of '") + (name)) + std::string("' in same scope")), tok);
                return;
            }
        }
        scope.names.push_back(name);
        scope.symbols.push_back(Symbol(name, t, kind, is_ref, is_mutable));
    }

    bool resolve(const std::string& name) {
        int64_t i = (static_cast<int64_t>(this->scopes.size()) - INT64_C(1));
        while ((i >= INT64_C(0))) {
            auto& scope = this->scopes[i];
            for (const auto& n : scope.names) {
                if ((n == name)) {
                    return true;
                }
            }
            i = (i - INT64_C(1));
        }
        if ((this->known_funcs.count(name) > 0)) {
            return true;
        }
        if ((this->known_classes.count(name) > 0)) {
            return true;
        }
        if ((this->known_enums.count(name) > 0)) {
            return true;
        }
        if (name.starts_with(std::string("__"))) {
            return true;
        }
        int64_t angle = lv_index_of(name, std::string("<"));
        if ((angle >= INT64_C(0))) {
            std::string base = name.substr(INT64_C(0), (angle) - (INT64_C(0)));
            return (*this).resolve(base);
        }
        return false;
    }

    Symbol lookup(const std::string& name) {
        int64_t i = (static_cast<int64_t>(this->scopes.size()) - INT64_C(1));
        while ((i >= INT64_C(0))) {
            auto& scope = this->scopes[i];
            for (int64_t j = INT64_C(0); j < static_cast<int64_t>(scope.names.size()); j++) {
                if ((scope.names[j] == name)) {
                    return scope.symbols[j];
                }
            }
            i = (i - INT64_C(1));
        }
        return Symbol(name, TypeNode::make_Auto(), std::string("unknown"), false, false);
    }

    bool types_compatible(const TypeNode& expected, const TypeNode& actual) {
        {
            const auto& _match_70 = actual;
            if (std::holds_alternative<std::decay_t<decltype(_match_70)>::Array>(_match_70._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_70)>::Array>(_match_70._data);
                auto& a_inner = *_v.inner;
                {
                    const auto& _match_71 = a_inner;
                    if (std::holds_alternative<std::decay_t<decltype(_match_71)>::Auto>(_match_71._data)) {
                        {
                            const auto& _match_72 = expected;
                            if (std::holds_alternative<std::decay_t<decltype(_match_72)>::Array>(_match_72._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_72)>::Array>(_match_72._data);
                                auto& e_inner = *_v.inner;
                                return true;
                            }
                            else if (std::holds_alternative<std::decay_t<decltype(_match_72)>::HashSet>(_match_72._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_72)>::HashSet>(_match_72._data);
                                auto& e_inner = *_v.inner;
                                return true;
                            }
                            else if (std::holds_alternative<std::decay_t<decltype(_match_72)>::HashMap>(_match_72._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_72)>::HashMap>(_match_72._data);
                                auto& ek = *_v.key_type;
                                auto& ev = *_v.value_type;
                                return true;
                            }
                            else {
                                /* pass */
                            }
                        }
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else {
                /* pass */
            }
        }
        {
            const auto& _match_73 = expected;
            if (std::holds_alternative<std::decay_t<decltype(_match_73)>::Array>(_match_73._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_73)>::Array>(_match_73._data);
                auto& e_inner = *_v.inner;
                {
                    const auto& _match_74 = actual;
                    if (std::holds_alternative<std::decay_t<decltype(_match_74)>::Array>(_match_74._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_74)>::Array>(_match_74._data);
                        auto& a_inner = *_v.inner;
                        return (*this).types_compatible(e_inner, a_inner);
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else {
                /* pass */
            }
        }
        {
            const auto& _match_75 = expected;
            if (std::holds_alternative<std::decay_t<decltype(_match_75)>::Auto>(_match_75._data)) {
                return true;
            }
            else if (_match_75._tag == "None") {
                return true;
            }
            else {
                /* pass */
            }
        }
        {
            const auto& _match_76 = actual;
            if (std::holds_alternative<std::decay_t<decltype(_match_76)>::Auto>(_match_76._data)) {
                return true;
            }
            else if (_match_76._tag == "None") {
                return true;
            }
            else {
                /* pass */
            }
        }
        std::string e = type_to_display(expected);
        std::string a = type_to_display(actual);
        if ((e == a)) {
            return true;
        }
        bool e_is_int = is_integer_type(expected);
        bool a_is_int = is_integer_type(actual);
        if (e_is_int && a_is_int) {
            return true;
        }
        bool e_is_float = is_float_type(expected);
        bool a_is_float = is_float_type(actual);
        if (e_is_float && a_is_float) {
            return true;
        }
        if (e_is_float && a_is_int) {
            return true;
        }
        {
            const auto& _match_77 = expected;
            if (std::holds_alternative<std::decay_t<decltype(_match_77)>::CString>(_match_77._data)) {
                {
                    const auto& _match_78 = actual;
                    if (std::holds_alternative<std::decay_t<decltype(_match_78)>::Str>(_match_78._data)) {
                        return true;
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_77)>::Str>(_match_77._data)) {
                {
                    const auto& _match_79 = actual;
                    if (std::holds_alternative<std::decay_t<decltype(_match_79)>::CString>(_match_79._data)) {
                        return true;
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else {
                /* pass */
            }
        }
        {
            const auto& _match_80 = expected;
            if (std::holds_alternative<std::decay_t<decltype(_match_80)>::Nullable>(_match_80._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_80)>::Nullable>(_match_80._data);
                auto& inner = *_v.inner;
                {
                    const auto& _match_81 = actual;
                    if (std::holds_alternative<std::decay_t<decltype(_match_81)>::NullType>(_match_81._data)) {
                        return true;
                    }
                    else {
                        return (*this).types_compatible(inner, actual);
                    }
                }
            }
            else {
                /* pass */
            }
        }
        {
            const auto& _match_82 = expected;
            if (std::holds_alternative<std::decay_t<decltype(_match_82)>::Ptr>(_match_82._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_82)>::Ptr>(_match_82._data);
                auto& inner = *_v.inner;
                {
                    const auto& _match_83 = actual;
                    if (std::holds_alternative<std::decay_t<decltype(_match_83)>::NullType>(_match_83._data)) {
                        return true;
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else {
                /* pass */
            }
        }
        return false;
    }

    TypeNode infer_type(const Expr& e) {
        {
            const auto& _match_84 = e;
            if (_match_84._tag == "None") {
                return TypeNode::make_None();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_84)>::Literal>(_match_84._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_84)>::Literal>(_match_84._data);
                auto& kind = _v.kind;
                auto& value = _v.value;
                if ((kind == std::string("int"))) {
                    return TypeNode::make_Int();
                }
                else {
                    if ((kind == std::string("float"))) {
                        return TypeNode::make_Float();
                    }
                    else {
                        if ((kind == std::string("string"))) {
                            return TypeNode::make_Str();
                        }
                        else {
                            if ((kind == std::string("bool"))) {
                                return TypeNode::make_Bool();
                            }
                            else {
                                if ((kind == std::string("null"))) {
                                    return TypeNode::make_NullType();
                                }
                            }
                        }
                    }
                }
                return TypeNode::make_Auto();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_84)>::Variable>(_match_84._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_84)>::Variable>(_match_84._data);
                auto& name = _v.name;
                auto sym = (*this).lookup(name.lexeme);
                return sym.sym_type;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_84)>::Binary>(_match_84._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_84)>::Binary>(_match_84._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                auto lt = (*this).infer_type(left);
                auto rt = (*this).infer_type(right);
                if ((op.token_type == TK_EQUAL_EQUAL) || (op.token_type == TK_BANG_EQUAL) || (op.token_type == TK_LESS) || (op.token_type == TK_LESS_EQUAL) || (op.token_type == TK_GREATER) || (op.token_type == TK_GREATER_EQUAL)) {
                    return TypeNode::make_Bool();
                }
                {
                    const auto& _match_85 = lt;
                    if (std::holds_alternative<std::decay_t<decltype(_match_85)>::Str>(_match_85._data)) {
                        return TypeNode::make_Str();
                    }
                    else {
                        /* pass */
                    }
                }
                {
                    const auto& _match_86 = lt;
                    if (std::holds_alternative<std::decay_t<decltype(_match_86)>::Float>(_match_86._data)) {
                        return TypeNode::make_Float();
                    }
                    else {
                        /* pass */
                    }
                }
                {
                    const auto& _match_87 = rt;
                    if (std::holds_alternative<std::decay_t<decltype(_match_87)>::Float>(_match_87._data)) {
                        return TypeNode::make_Float();
                    }
                    else {
                        /* pass */
                    }
                }
                {
                    const auto& _match_88 = lt;
                    if (std::holds_alternative<std::decay_t<decltype(_match_88)>::Int>(_match_88._data)) {
                        return TypeNode::make_Int();
                    }
                    else {
                        /* pass */
                    }
                }
                return TypeNode::make_Auto();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_84)>::Unary>(_match_84._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_84)>::Unary>(_match_84._data);
                auto& op = _v.op;
                auto& right = *_v.right;
                if ((op.token_type == TK_BANG) || (op.token_type == TK_NOT)) {
                    return TypeNode::make_Bool();
                }
                return (*this).infer_type(right);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_84)>::Logical>(_match_84._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_84)>::Logical>(_match_84._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                return TypeNode::make_Bool();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_84)>::Call>(_match_84._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_84)>::Call>(_match_84._data);
                auto& callee = *_v.callee;
                auto& paren = _v.paren;
                auto& args = _v.args;
                auto& arg_names = _v.arg_names;
                return (*this).infer_call_type(callee);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_84)>::Grouping>(_match_84._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_84)>::Grouping>(_match_84._data);
                auto& inner = *_v.inner;
                return (*this).infer_type(inner);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_84)>::Index>(_match_84._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_84)>::Index>(_match_84._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                auto ot = (*this).infer_type(object);
                {
                    const auto& _match_89 = ot;
                    if (std::holds_alternative<std::decay_t<decltype(_match_89)>::Array>(_match_89._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_89)>::Array>(_match_89._data);
                        auto& inner = *_v.inner;
                        return inner;
                    }
                    else {
                        return TypeNode::make_Auto();
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_84)>::Vector>(_match_84._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_84)>::Vector>(_match_84._data);
                auto& elements = _v.elements;
                if ((static_cast<int64_t>(elements.size()) > INT64_C(0))) {
                    auto inner = (*this).infer_type(elements[INT64_C(0)]);
                    return TypeNode::make_Array(inner);
                }
                return TypeNode::make_Array(TypeNode::make_Auto());
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_84)>::Cast>(_match_84._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_84)>::Cast>(_match_84._data);
                auto& expr = *_v.expr;
                auto& target_type = _v.target_type;
                return target_type;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_84)>::This>(_match_84._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_84)>::This>(_match_84._data);
                auto& kw = _v.keyword;
                return TypeNode::make_Custom(this->current_class_name, {});
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_84)>::Own>(_match_84._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_84)>::Own>(_match_84._data);
                auto& expr = *_v.expr;
                return (*this).infer_type(expr);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_84)>::AddressOf>(_match_84._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_84)>::AddressOf>(_match_84._data);
                auto& expr = *_v.expr;
                return TypeNode::make_Ptr((*this).infer_type(expr));
            }
            else {
                return TypeNode::make_Auto();
            }
        }
    }

    TypeNode infer_call_type(const Expr& callee) {
        {
            const auto& _match_90 = callee;
            if (std::holds_alternative<std::decay_t<decltype(_match_90)>::Variable>(_match_90._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_90)>::Variable>(_match_90._data);
                auto& name = _v.name;
                if ((this->known_funcs.count(name.lexeme) > 0)) {
                    ExternFn fi = this->known_funcs[name.lexeme];
                    return fi.return_type;
                }
                if ((this->known_classes.count(name.lexeme) > 0)) {
                    return TypeNode::make_Custom(name.lexeme, {});
                }
                if ((this->known_enums.count(name.lexeme) > 0)) {
                    return TypeNode::make_Custom(name.lexeme, {});
                }
                return TypeNode::make_Auto();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_90)>::Get>(_match_90._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_90)>::Get>(_match_90._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                return TypeNode::make_Auto();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_90)>::StaticGet>(_match_90._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_90)>::StaticGet>(_match_90._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                {
                    const auto& _match_91 = object;
                    if (std::holds_alternative<std::decay_t<decltype(_match_91)>::Variable>(_match_91._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_91)>::Variable>(_match_91._data);
                        auto& obj_name = _v.name;
                        if ((this->known_enums.count(obj_name.lexeme) > 0)) {
                            return TypeNode::make_Custom(obj_name.lexeme, {});
                        }
                    }
                    else {
                        /* pass */
                    }
                }
                return TypeNode::make_Auto();
            }
            else {
                return TypeNode::make_Auto();
            }
        }
    }

    void register_builtins() {
        std::vector<std::string> builtins = std::vector{std::string("print"), std::string("println"), std::string("lv_assert"), std::string("to_string"), std::string("input"), std::string("typeof"), std::string("len"), std::string("exit"), std::string("abs"), std::string("cast")};
        std::vector<Param> empty_params = {};
        for (const auto& name : builtins) {
            std::vector<Expr> empty_defaults = {};
            this->known_funcs[name] = ExternFn(name, name, TypeNode::make_Auto(), empty_params, empty_defaults);
        }
    }

    void check(const std::vector<Stmt>& stmts) {
        (*this).register_builtins();
        (*this).push_scope();
        for (const auto& s : stmts) {
            (*this).collect_decl(s);
        }
        for (const auto& s : stmts) {
            (*this).check_stmt(s);
        }
        (*this).pop_scope();
    }

    void collect_decl(const Stmt& s) {
        {
            const auto& _match_92 = s;
            if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Function>(_match_92._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_92)>::Function>(_match_92._data);
                auto& name = _v.name;
                auto& params = _v.params;
                auto& return_type = _v.return_type;
                auto& body = _v.body;
                auto& is_inline = _v.is_inline;
                auto& comptime_mode = _v.comptime_mode;
                auto& is_static = _v.is_static;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                auto& param_defaults = _v.param_defaults;
                this->known_funcs[name.lexeme] = ExternFn(name.lexeme, name.lexeme, return_type, params, param_defaults);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Class>(_match_92._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_92)>::Class>(_match_92._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                this->known_classes[name.lexeme] = body;
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Enum>(_match_92._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_92)>::Enum>(_match_92._data);
                auto& name = _v.name;
                auto& variants = _v.variants;
                auto& methods = _v.methods;
                auto& visibility = _v.visibility;
                auto& enum_tp = _v.type_params;
                this->known_enums[name.lexeme] = variants;
                for (const auto& method : methods) {
                    {
                        const auto& _match_93 = method;
                        if (std::holds_alternative<std::decay_t<decltype(_match_93)>::Function>(_match_93._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_93)>::Function>(_match_93._data);
                            auto& method_name = _v.name;
                            auto& params = _v.params;
                            auto& return_type = _v.return_type;
                            auto& body = _v.body;
                            auto& is_inline = _v.is_inline;
                            auto& comptime_mode = _v.comptime_mode;
                            auto& is_static = _v.is_static;
                            auto& method_visibility = _v.visibility;
                            auto& type_params = _v.type_params;
                            auto& param_defaults = _v.param_defaults;
                            std::vector<Param> lowered_params = std::vector{Param(Token(TK_IDENTIFIER, std::string("self"), name.line, name.col), TypeNode::make_Custom(name.lexeme, {}), true, true)};
                            for (const auto& param : params) {
                                lowered_params.push_back(param);
                            }
                            std::string lowered_name = ((((std::string("") + (name.lexeme)) + std::string("__")) + (method_name.lexeme)) + std::string(""));
                            this->known_funcs[lowered_name] = ExternFn(lowered_name, lowered_name, return_type, lowered_params, param_defaults);
                        }
                        else {
                            /* pass */
                        }
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Const>(_match_92._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_92)>::Const>(_match_92._data);
                auto& name = _v.name;
                auto& const_type = _v.const_type;
                auto& value = _v.value;
                auto& visibility = _v.visibility;
                auto& comptime_mode = _v.comptime_mode;
                (*this).declare(name.lexeme, const_type, std::string("const"), false, false, name);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Struct>(_match_92._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_92)>::Struct>(_match_92._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                auto& struct_tp = _v.type_params;
                this->known_classes[name.lexeme] = body;
                for (const auto& st : body) {
                    {
                        const auto& _match_94 = st;
                        if (std::holds_alternative<std::decay_t<decltype(_match_94)>::Function>(_match_94._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_94)>::Function>(_match_94._data);
                            auto& fname = _v.name;
                            auto& fparams = _v.params;
                            auto& fret = _v.return_type;
                            auto& fbody = _v.body;
                            auto& fi = _v.is_inline;
                            auto& fc = _v.comptime_mode;
                            auto& fs = _v.is_static;
                            auto& fv = _v.visibility;
                            auto& ftp = _v.type_params;
                            auto& f_defaults = _v.param_defaults;
                            if ((fname.lexeme == std::string("constructor"))) {
                                this->known_funcs[name.lexeme] = ExternFn(name.lexeme, name.lexeme, TypeNode::make_Custom(name.lexeme, {}), fparams, f_defaults);
                            }
                        }
                        else {
                            /* pass */
                        }
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Namespace>(_match_92._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_92)>::Namespace>(_match_92._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                for (const auto& ns_stmt : body) {
                    (*this).collect_decl(ns_stmt);
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_92)>::Extern>(_match_92._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_92)>::Extern>(_match_92._data);
                auto& header = _v.header;
                auto& import_path = _v.import_path;
                auto& link_lib = _v.link_lib;
                auto& types = _v.types;
                auto& functions = _v.functions;
                for (const auto& et : types) {
                    std::vector<Stmt> empty_body = {};
                    this->known_classes[et.lavina_name] = empty_body;
                }
                for (const auto& ef : functions) {
                    this->known_funcs[ef.name] = ef;
                }
            }
            else {
                /* pass */
            }
        }
    }

    void check_stmt(const Stmt& s) {
        {
            const auto& _match_95 = s;
            if (std::holds_alternative<std::decay_t<decltype(_match_95)>::ExprStmt>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::ExprStmt>(_match_95._data);
                auto& expr = _v.expr;
                (*this).check_expr(expr);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Let>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Let>(_match_95._data);
                auto& name = _v.name;
                auto& var_type = _v.var_type;
                auto& initializer = _v.initializer;
                auto& visibility = _v.visibility;
                auto& is_ref = _v.is_ref;
                auto& is_mut = _v.is_mut;
                (*this).check_expr(initializer);
                auto init_type = (*this).infer_type(initializer);
                {
                    const auto& _match_96 = var_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_96)>::Auto>(_match_96._data)) {
                        /* pass */
                    }
                    else if (_match_96._tag == "None") {
                        /* pass */
                    }
                    else {
                        if ((!(*this).types_compatible(var_type, init_type))) {
                            (*this).error(((((std::string("Cannot assign ") + (type_to_display(init_type))) + std::string(" to ")) + (type_to_display(var_type))) + std::string("")), name);
                        }
                    }
                }
                (*this).declare(name.lexeme, var_type, std::string("var"), is_ref, true, name);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Const>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Const>(_match_95._data);
                auto& name = _v.name;
                auto& const_type = _v.const_type;
                auto& value = _v.value;
                auto& visibility = _v.visibility;
                auto& comptime_mode = _v.comptime_mode;
                (*this).check_expr(value);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Return>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Return>(_match_95._data);
                auto& keyword = _v.keyword;
                auto& value = _v.value;
                (*this).check_expr(value);
                {
                    const auto& _match_97 = this->current_return_type;
                    if (_match_97._tag == "None") {
                        /* pass */
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_97)>::Void>(_match_97._data)) {
                        {
                            const auto& _match_98 = value;
                            if (_match_98._tag == "None") {
                                /* pass */
                            }
                            else {
                                (*this).error(std::string("Cannot return a value from void function"), keyword);
                            }
                        }
                    }
                    else {
                        auto val_type = (*this).infer_type(value);
                        if ((!(*this).types_compatible(this->current_return_type, val_type))) {
                            (*this).error(((((std::string("Return type mismatch: expected ") + (type_to_display(this->current_return_type))) + std::string(", got ")) + (type_to_display(val_type))) + std::string("")), keyword);
                        }
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::If>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::If>(_match_95._data);
                auto& condition = _v.condition;
                auto& then_branch = *_v.then_branch;
                auto& else_branch = *_v.else_branch;
                (*this).check_expr(condition);
                (*this).check_stmt(then_branch);
                (*this).check_stmt(else_branch);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::While>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::While>(_match_95._data);
                auto& condition = _v.condition;
                auto& body = *_v.body;
                (*this).check_expr(condition);
                (*this).check_stmt(body);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::For>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::For>(_match_95._data);
                auto& item_name = _v.item_name;
                auto& collection = _v.collection;
                auto& body = *_v.body;
                auto& is_ref = _v.is_ref;
                auto& is_mut = _v.is_mut;
                (*this).check_expr(collection);
                (*this).push_scope();
                auto coll_type = (*this).infer_type(collection);
                auto item_type = TypeNode::make_Auto();
                {
                    const auto& _match_99 = coll_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_99)>::Array>(_match_99._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_99)>::Array>(_match_99._data);
                        auto& inner = *_v.inner;
                        item_type = inner;
                    }
                    else {
                        /* pass */
                    }
                }
                (*this).declare(item_name.lexeme, item_type, std::string("var"), is_ref, is_mut || (!is_ref), item_name);
                (*this).check_stmt(body);
                (*this).pop_scope();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Block>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Block>(_match_95._data);
                auto& statements = _v.statements;
                (*this).push_scope();
                for (const auto& st : statements) {
                    (*this).check_stmt(st);
                }
                (*this).pop_scope();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Function>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Function>(_match_95._data);
                auto& name = _v.name;
                auto& params = _v.params;
                auto& return_type = _v.return_type;
                auto& body = _v.body;
                auto& is_inline = _v.is_inline;
                auto& comptime_mode = _v.comptime_mode;
                auto& is_static = _v.is_static;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                auto& param_defaults = _v.param_defaults;
                (*this).check_function(name, params, return_type, body);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Class>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Class>(_match_95._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                (*this).check_class(name, body);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Struct>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Struct>(_match_95._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                auto& struct_tp = _v.type_params;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Enum>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Enum>(_match_95._data);
                auto& name = _v.name;
                auto& variants = _v.variants;
                auto& methods = _v.methods;
                auto& visibility = _v.visibility;
                auto& enum_tp = _v.type_params;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Match>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Match>(_match_95._data);
                auto& expr = _v.expr;
                auto& arm_patterns = _v.arm_patterns;
                auto& arm_bodies = _v.arm_bodies;
                (*this).check_expr(expr);
                (*this).check_match(expr, arm_patterns, arm_bodies);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Try>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Try>(_match_95._data);
                auto& try_body = *_v.try_body;
                auto& catch_body = *_v.catch_body;
                auto& exception_name = _v.exception_name;
                (*this).push_scope();
                (*this).check_stmt(try_body);
                (*this).pop_scope();
                (*this).push_scope();
                if ((exception_name != std::string(""))) {
                    (*this).declare(exception_name, TypeNode::make_Auto(), std::string("var"), false, false, Token(TK_IDENTIFIER, exception_name, INT64_C(0), INT64_C(0)));
                }
                (*this).check_stmt(catch_body);
                (*this).pop_scope();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Namespace>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Namespace>(_match_95._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                (*this).push_scope();
                for (const auto& ns_stmt : body) {
                    (*this).collect_decl(ns_stmt);
                }
                for (const auto& ns_stmt : body) {
                    (*this).check_stmt(ns_stmt);
                }
                (*this).pop_scope();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Import>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Import>(_match_95._data);
                auto& path = _v.path;
                auto& alias = _v.alias;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Break>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Break>(_match_95._data);
                auto& keyword = _v.keyword;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Continue>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Continue>(_match_95._data);
                auto& keyword = _v.keyword;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Pass>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Pass>(_match_95._data);
                auto& keyword = _v.keyword;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::CppBlock>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::CppBlock>(_match_95._data);
                auto& code = _v.code;
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_95)>::Extern>(_match_95._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_95)>::Extern>(_match_95._data);
                auto& header = _v.header;
                auto& import_path = _v.import_path;
                auto& link_lib = _v.link_lib;
                auto& types = _v.types;
                auto& functions = _v.functions;
                /* pass */
            }
            else {
                /* pass */
            }
        }
    }

    void declare_params(const std::vector<Param>& params) {
        for (const auto& p : params) {
            (*this).declare(p.name.lexeme, p.param_type, std::string("param"), p.is_ref, true, p.name);
        }
    }

    void check_function(const Token& name, const std::vector<Param>& params, const TypeNode& return_type, const std::vector<Stmt>& body) {
        auto saved_return = this->current_return_type;
        this->current_return_type = return_type;
        (*this).push_scope();
        (*this).declare_params(params);
        for (const auto& s : body) {
            (*this).check_stmt(s);
        }
        (*this).pop_scope();
        this->current_return_type = saved_return;
    }

    void check_class(const Token& name, const std::vector<Stmt>& body) {
        bool saved_in_class = this->in_class;
        std::string saved_class_name = this->current_class_name;
        this->in_class = true;
        this->current_class_name = name.lexeme;
        (*this).push_scope();
        (*this).declare(std::string("this"), TypeNode::make_Custom(name.lexeme, {}), std::string("var"), false, false, name);
        for (const auto& s : body) {
            {
                const auto& _match_100 = s;
                if (std::holds_alternative<std::decay_t<decltype(_match_100)>::Function>(_match_100._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_100)>::Function>(_match_100._data);
                    auto& fname = _v.name;
                    auto& params = _v.params;
                    auto& return_type = _v.return_type;
                    auto& fbody = _v.body;
                    auto& is_inline = _v.is_inline;
                    auto& comptime_mode = _v.comptime_mode;
                    auto& is_static = _v.is_static;
                    auto& visibility = _v.visibility;
                    auto& fn_tp = _v.type_params;
                    auto& fn_defaults = _v.param_defaults;
                    (*this).check_function(fname, params, return_type, fbody);
                }
                else if (std::holds_alternative<std::decay_t<decltype(_match_100)>::Let>(_match_100._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_100)>::Let>(_match_100._data);
                    auto& lname = _v.name;
                    auto& var_type = _v.var_type;
                    auto& initializer = _v.initializer;
                    auto& visibility = _v.visibility;
                    auto& is_ref = _v.is_ref;
                    auto& is_mut = _v.is_mut;
                    (*this).check_expr(initializer);
                }
                else {
                    (*this).check_stmt(s);
                }
            }
        }
        (*this).pop_scope();
        this->in_class = saved_in_class;
        this->current_class_name = saved_class_name;
    }

    void check_expr(const Expr& e) {
        {
            const auto& _match_101 = e;
            if (_match_101._tag == "None") {
                /* pass */
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Variable>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Variable>(_match_101._data);
                auto& name = _v.name;
                if ((!(*this).resolve(name.lexeme))) {
                    (*this).error(((std::string("Undefined variable '") + (name.lexeme)) + std::string("'")), name);
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Binary>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Binary>(_match_101._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                (*this).check_expr(left);
                (*this).check_expr(right);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Unary>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Unary>(_match_101._data);
                auto& op = _v.op;
                auto& right = *_v.right;
                (*this).check_expr(right);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Logical>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Logical>(_match_101._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                (*this).check_expr(left);
                (*this).check_expr(right);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Grouping>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Grouping>(_match_101._data);
                auto& inner = *_v.inner;
                (*this).check_expr(inner);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Call>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Call>(_match_101._data);
                auto& callee = *_v.callee;
                auto& paren = _v.paren;
                auto& args = _v.args;
                auto& arg_names = _v.arg_names;
                (*this).check_expr(callee);
                for (const auto& a : args) {
                    (*this).check_expr(a);
                }
                (*this).check_call_args(callee, args, arg_names, paren);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Assign>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Assign>(_match_101._data);
                auto& name = _v.name;
                auto& value = *_v.value;
                (*this).check_expr(value);
                if ((!(*this).resolve(name.lexeme))) {
                    (*this).error(((std::string("Undefined variable '") + (name.lexeme)) + std::string("'")), name);
                }
                else {
                    auto sym = (*this).lookup(name.lexeme);
                    if ((sym.kind == std::string("const"))) {
                        (*this).error(((std::string("Cannot assign to constant '") + (name.lexeme)) + std::string("'")), name);
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Index>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Index>(_match_101._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                (*this).check_expr(object);
                (*this).check_expr(index);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::IndexSet>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::IndexSet>(_match_101._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                auto& value = *_v.value;
                (*this).check_expr(object);
                (*this).check_expr(index);
                (*this).check_expr(value);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Vector>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Vector>(_match_101._data);
                auto& elements = _v.elements;
                for (const auto& el : elements) {
                    (*this).check_expr(el);
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Map>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Map>(_match_101._data);
                auto& keys = _v.keys;
                auto& values = _v.values;
                for (const auto& k : keys) {
                    (*this).check_expr(k);
                }
                for (const auto& v : values) {
                    (*this).check_expr(v);
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Get>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Get>(_match_101._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                (*this).check_expr(object);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Set>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Set>(_match_101._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                auto& value = *_v.value;
                (*this).check_expr(object);
                (*this).check_expr(value);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::StaticGet>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::StaticGet>(_match_101._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                {
                    const auto& _match_102 = object;
                    if (std::holds_alternative<std::decay_t<decltype(_match_102)>::Variable>(_match_102._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_102)>::Variable>(_match_102._data);
                        auto& tok = _v.name;
                        /* pass */
                    }
                    else {
                        (*this).check_expr(object);
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Cast>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Cast>(_match_101._data);
                auto& expr = *_v.expr;
                auto& target_type = _v.target_type;
                (*this).check_expr(expr);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Throw>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Throw>(_match_101._data);
                auto& expr = *_v.expr;
                (*this).check_expr(expr);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Range>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Range>(_match_101._data);
                auto& start = *_v.start;
                auto& end = *_v.end;
                (*this).check_expr(start);
                (*this).check_expr(end);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Lambda>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Lambda>(_match_101._data);
                auto& params = _v.params;
                auto& body = *_v.body;
                (*this).push_scope();
                (*this).declare_params(params);
                (*this).check_expr(body);
                (*this).pop_scope();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::BlockLambda>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::BlockLambda>(_match_101._data);
                auto& params = _v.params;
                auto& body_id = _v.body_id;
                (*this).push_scope();
                (*this).declare_params(params);
                (*this).pop_scope();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::Own>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::Own>(_match_101._data);
                auto& expr = *_v.expr;
                (*this).check_expr(expr);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_101)>::AddressOf>(_match_101._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_101)>::AddressOf>(_match_101._data);
                auto& expr = *_v.expr;
                (*this).check_expr(expr);
            }
            else {
                /* pass */
            }
        }
    }

    void check_call_args(const Expr& callee, const std::vector<Expr>& args, const std::vector<std::string>& arg_names, const Token& paren) {
        {
            const auto& _match_103 = callee;
            if (std::holds_alternative<std::decay_t<decltype(_match_103)>::Variable>(_match_103._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_103)>::Variable>(_match_103._data);
                auto& name = _v.name;
                if ((this->known_funcs.count(name.lexeme) > 0)) {
                    ExternFn fi = this->known_funcs[name.lexeme];
                    if ((static_cast<int64_t>(fi.params.size()) == INT64_C(0))) {
                        return;
                    }
                    int64_t required = INT64_C(0);
                    int64_t di = INT64_C(0);
                    while ((di < static_cast<int64_t>(fi.param_defaults.size()))) {
                        {
                            const auto& _match_104 = fi.param_defaults[di];
                            if (_match_104._tag == "None") {
                                required = (required + INT64_C(1));
                            }
                            else {
                                /* pass */
                            }
                        }
                        di = (di + INT64_C(1));
                    }
                    if ((static_cast<int64_t>(fi.param_defaults.size()) == INT64_C(0))) {
                        required = static_cast<int64_t>(fi.params.size());
                    }
                    if ((static_cast<int64_t>(args.size()) < required) || (static_cast<int64_t>(args.size()) > static_cast<int64_t>(fi.params.size()))) {
                        if ((required == static_cast<int64_t>(fi.params.size()))) {
                            (*this).error(((((((std::string("Function '") + (name.lexeme)) + std::string("' expects ")) + (static_cast<int64_t>(fi.params.size()))) + std::string(" args, got ")) + (static_cast<int64_t>(args.size()))) + std::string("")), paren);
                        }
                        else {
                            (*this).error(((((((((std::string("Function '") + (name.lexeme)) + std::string("' expects ")) + (required)) + std::string("-")) + (static_cast<int64_t>(fi.params.size()))) + std::string(" args, got ")) + (static_cast<int64_t>(args.size()))) + std::string("")), paren);
                        }
                    }
                    return;
                }
            }
            else {
                /* pass */
            }
        }
    }

    void check_match(const Expr& expr, const std::vector<MatchArm>& arm_patterns, const std::vector<Stmt>& arm_bodies) {
        bool has_wildcard = false;
        for (int64_t i = INT64_C(0); i < static_cast<int64_t>(arm_patterns.size()); i++) {
            auto arm = arm_patterns[i];
            if ((arm.pattern_name == std::string("_"))) {
                has_wildcard = true;
            }
            else {
                std::string enum_name = find_enum_for_variant(this->known_enums, arm.pattern_name);
                if ((enum_name == std::string(""))) {
                    /* pass */
                }
            }
            (*this).push_scope();
            for (const auto& b : arm.bindings) {
                (*this).declare(b, TypeNode::make_Auto(), std::string("var"), false, false, Token(TK_IDENTIFIER, b, INT64_C(0), INT64_C(0)));
            }
            (*this).check_stmt(arm_bodies[i]);
            (*this).pop_scope();
        }
    }

};

std::vector<std::string> run_checker(const std::vector<Stmt>& stmts) {
    auto checker = Checker();
    checker.check(stmts);
    for (const auto& w : checker.warnings) {
        print(w);
    }
    return checker.errors;
}

struct CppParam {
    std::string name;
    std::string param_type;

};

struct CppLocal {
    std::string name;
    std::string local_type;

};

struct CppFunction {
    std::string name;
    std::string return_type;
    std::vector<CppParam> params;
    std::vector<CppLocal> locals;
    std::vector<std::string> body_lines;
    bool is_entry;

};

struct CppField {
    std::string name;
    std::string field_type;

};

struct CppStruct {
    std::string name;
    std::vector<CppField> fields;
    std::vector<CppParam> constructor_params;
    std::vector<std::string> constructor_defaults;
    std::vector<std::string> init_fields;
    std::vector<std::string> constructor_body_lines;

};

struct CppEnumVariant {
    std::string name;
    std::vector<CppField> fields;

};

struct CppEnum {
    std::string name;
    std::vector<CppEnumVariant> variants;
    bool has_to_string;

};

struct CppModule {
    std::vector<std::string> includes;
    std::vector<CppStruct> structs;
    std::vector<CppEnum> enums;
    std::vector<CppFunction> functions;

};

std::string cpp_escape_string(std::string value) {
    return lv_replace(lv_replace(lv_replace(lv_replace(value, std::string("\\"), std::string("\\\\")), std::string("\""), std::string("\\\"")), std::string("\n"), std::string("\\n")), std::string("\t"), std::string("\\t"));
}

std::string cpp_operator(std::string op) {
    if ((op == std::string("and"))) {
        return std::string("&&");
    }
    else {
        if ((op == std::string("or"))) {
            return std::string("||");
        }
        else {
            if ((op == std::string("not"))) {
                return std::string("!");
            }
        }
    }
    return op;
}

std::string cpp_safe_name(std::string name) {
    if (name.starts_with(std::string("%"))) {
        return ((std::string("tmp_") + (name.substr(INT64_C(1), (static_cast<int64_t>(name.size())) - (INT64_C(1))))) + std::string(""));
    }
    return name;
}

std::string cpp_literal(const IrInst& inst) {
    if ((ir_inst_literal_kind(inst) == std::string("string"))) {
        return ((std::string("\"") + (cpp_escape_string(ir_inst_literal_value(inst)))) + std::string("\""));
    }
    else {
        if ((ir_inst_literal_kind(inst) == std::string("bool"))) {
            return ir_inst_literal_value(inst);
        }
        else {
            if ((ir_inst_literal_kind(inst) == std::string("null"))) {
                return std::string("nullptr");
            }
        }
    }
    return ir_inst_literal_value(inst);
}

std::string cpp_type_from_ir(const IrType& t) {
    if (ir_type_is_void(t)) {
        return std::string("void");
    }
    else {
        if (ir_type_is_bool(t)) {
            return std::string("bool");
        }
        else {
            if (ir_type_is_int(t)) {
                return std::string("int64_t");
            }
            else {
                if (ir_type_is_float(t)) {
                    return std::string("double");
                }
                else {
                    if (ir_type_is_string(t)) {
                        return std::string("std::string");
                    }
                    else {
                        if (ir_type_is_bytes(t)) {
                            return std::string("lv_bytes");
                        }
                        else {
                            if ((t.name == std::string("int8"))) {
                                return std::string("int8_t");
                            }
                            else {
                                if ((t.name == std::string("int16"))) {
                                    return std::string("int16_t");
                                }
                                else {
                                    if ((t.name == std::string("int32"))) {
                                        return std::string("int32_t");
                                    }
                                    else {
                                        if ((t.name == std::string("float32"))) {
                                            return std::string("float");
                                        }
                                        else {
                                            if ((t.name == std::string("usize"))) {
                                                return std::string("size_t");
                                            }
                                            else {
                                                if ((t.name == std::string("*char"))) {
                                                    return std::string("const char*");
                                                }
                                                else {
                                                    if (ir_type_is_array(t)) {
                                                        std::string inner = ir_array_inner_type(t);
                                                        return ((std::string("std::vector<") + (cpp_type_from_ir(IrType(inner)))) + std::string(">"));
                                                    }
                                                    else {
                                                        if (ir_type_is_set(t)) {
                                                            std::string inner = ir_set_inner_type(t);
                                                            return ((std::string("std::unordered_set<") + (cpp_type_from_ir(IrType(inner)))) + std::string(">"));
                                                        }
                                                        else {
                                                            if (ir_type_is_map(t)) {
                                                                std::string key_type = ir_map_key_type_name(t);
                                                                std::string value_type = ir_map_value_type_name(t);
                                                                return ((((std::string("std::unordered_map<") + (cpp_type_from_ir(IrType(key_type)))) + std::string(", ")) + (cpp_type_from_ir(IrType(value_type)))) + std::string(">"));
                                                            }
                                                            else {
                                                                if (ir_type_is_pair(t)) {
                                                                    std::vector<std::string> parts = ir_pair_type_args(t);
                                                                    if ((static_cast<int64_t>(parts.size()) == INT64_C(2))) {
                                                                        return ((((std::string("std::pair<") + (cpp_type_from_ir(IrType(parts[INT64_C(0)])))) + std::string(", ")) + (cpp_type_from_ir(IrType(parts[INT64_C(1)])))) + std::string(">"));
                                                                    }
                                                                    return t.name;
                                                                }
                                                                else {
                                                                    if (ir_type_is_ptr(t)) {
                                                                        return ((std::string("") + (cpp_type_from_ir(IrType(ir_ptr_inner_type(t))))) + std::string("*"));
                                                                    }
                                                                    else {
                                                                        if (ir_type_is_nullable(t)) {
                                                                            std::string inner = ir_nullable_inner_type(t);
                                                                            return ((std::string("std::optional<") + (cpp_type_from_ir(IrType(inner)))) + std::string(">"));
                                                                        }
                                                                        else {
                                                                            if (ir_type_is_closure(t)) {
                                                                                return std::string("auto");
                                                                            }
                                                                            else {
                                                                                if (ir_type_is_auto(t)) {
                                                                                    return std::string("auto");
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return t.name;
}

std::string cpp_type_from_type_node(const TypeNode& t) {
    {
        const auto& _match_105 = t;
        if (std::holds_alternative<std::decay_t<decltype(_match_105)>::Int>(_match_105._data)) {
            return std::string("int64_t");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_105)>::Float>(_match_105._data)) {
            return std::string("double");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_105)>::Str>(_match_105._data)) {
            return std::string("std::string");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_105)>::Bool>(_match_105._data)) {
            return std::string("bool");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_105)>::Void>(_match_105._data)) {
            return std::string("void");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_105)>::Int8>(_match_105._data)) {
            return std::string("int8_t");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_105)>::Int16>(_match_105._data)) {
            return std::string("int16_t");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_105)>::Int32>(_match_105._data)) {
            return std::string("int32_t");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_105)>::Float32>(_match_105._data)) {
            return std::string("float");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_105)>::USize>(_match_105._data)) {
            return std::string("size_t");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_105)>::Bytes>(_match_105._data)) {
            return std::string("lv_bytes");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_105)>::Custom>(_match_105._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_105)>::Custom>(_match_105._data);
            auto& name = _v.name;
            auto& type_args = _v.type_args;
            if ((name == std::string("__closure"))) {
                return std::string("auto");
            }
            return type_to_cpp(t);
        }
        else {
            return type_to_cpp(t);
        }
    }
}

std::string cpp_assign_target(const IrInst& inst) {
    if ((ir_inst_dest(inst) != std::string("")) && ir_type_is_auto(ir_inst_value_type(inst))) {
        return ((std::string("auto ") + (cpp_safe_name(ir_inst_dest(inst)))) + std::string(""));
    }
    return cpp_safe_name(ir_inst_dest(inst));
}

void cpp_push_line(std::vector<std::string>& lines, std::string line) {
    lines.push_back(line);
}

IrClosureDef cpp_find_closure(const std::vector<IrClosureDef>& closures, std::string name) {
    std::vector<IrParam> empty_params = {};
    for (const auto& closure : closures) {
        if ((closure.name == name)) {
            return closure;
        }
    }
    return IrClosureDef(std::string(""), std::string(""), empty_params, empty_params, IrClosureSig({}, ir_auto_type()));
}

IrClosureDef cpp_find_closure_by_helper(const std::vector<IrClosureDef>& closures, std::string helper_name) {
    std::vector<IrParam> empty_params = {};
    for (const auto& closure : closures) {
        if ((closure.helper_name == helper_name)) {
            return closure;
        }
    }
    return IrClosureDef(std::string(""), std::string(""), empty_params, empty_params, IrClosureSig({}, ir_auto_type()));
}

std::string cpp_render_closure_inst(const IrInst& inst, const std::vector<IrClosureDef>& closures) {
    auto closure = cpp_find_closure(closures, ir_inst_closure_name(inst));
    std::vector<std::string> capture_parts = {};
    for (const auto& arg : ir_inst_args(inst)) {
        capture_parts.push_back(((std::string("&") + (cpp_safe_name(arg))) + std::string("")));
    }
    std::string capture_list = std::string("[]");
    if ((static_cast<int64_t>(capture_parts.size()) > INT64_C(0))) {
        capture_list = ((std::string("[") + (lv_join(capture_parts, std::string(", ")))) + std::string("]"));
    }
    std::vector<std::string> param_parts = {};
    std::vector<std::string> call_parts = {};
    for (const auto& capture : ir_closure_capture_params(closure)) {
        call_parts.push_back(cpp_safe_name(capture.name));
    }
    for (const auto& param : ir_closure_lambda_params(closure)) {
        std::string param_name = cpp_safe_name(param.name);
        param_parts.push_back(((((std::string("") + (cpp_type_from_ir(param.param_type))) + std::string(" ")) + (param_name)) + std::string("")));
        call_parts.push_back(param_name);
    }
    std::string helper_call = ((((std::string("") + (ir_closure_helper_name(closure))) + std::string("(")) + (lv_join(call_parts, std::string(", ")))) + std::string(")"));
    if (ir_type_is_void(ir_closure_sig_return_type(ir_closure_signature(closure)))) {
        return ((((((((std::string("    auto ") + (cpp_safe_name(ir_inst_dest(inst)))) + std::string(" = ")) + (capture_list)) + std::string("(")) + (lv_join(param_parts, std::string(", ")))) + std::string(") { ")) + (helper_call)) + std::string("; };"));
    }
    return ((((((((std::string("    auto ") + (cpp_safe_name(ir_inst_dest(inst)))) + std::string(" = ")) + (capture_list)) + std::string("(")) + (lv_join(param_parts, std::string(", ")))) + std::string(") { return ")) + (helper_call)) + std::string("; };"));
}

std::string cpp_render_inst(const IrInst& inst, const std::vector<IrClosureDef>& closures) {
    if (ir_inst_is_const(inst)) {
        return ((((std::string("    ") + (cpp_assign_target(inst))) + std::string(" = ")) + (cpp_literal(inst))) + std::string(";"));
    }
    else {
        if (ir_inst_is_vector_literal(inst)) {
            std::vector<std::string> rendered = {};
            for (const auto& arg : ir_inst_args(inst)) {
                rendered.push_back(cpp_safe_name(arg));
            }
            return ((((std::string("    ") + (cpp_safe_name(ir_inst_dest(inst)))) + std::string(" = {")) + (lv_join(rendered, std::string(", ")))) + std::string("};"));
        }
        else {
            if (ir_inst_is_map_literal(inst)) {
                std::vector<std::string> rendered = {};
                int64_t i = INT64_C(0);
                auto args = ir_inst_args(inst);
                while ((i < static_cast<int64_t>(args.size()))) {
                    rendered.push_back(((((std::string("{") + (cpp_safe_name(args[i]))) + std::string(", ")) + (cpp_safe_name(args[(i + INT64_C(1))]))) + std::string("}")));
                    i = (i + INT64_C(2));
                }
                return ((((std::string("    ") + (cpp_safe_name(ir_inst_dest(inst)))) + std::string(" = {")) + (lv_join(rendered, std::string(", ")))) + std::string("};"));
            }
            else {
                if (ir_inst_is_move(inst)) {
                    return ((((std::string("    ") + (cpp_assign_target(inst))) + std::string(" = ")) + (cpp_safe_name(ir_inst_source_name(inst)))) + std::string(";"));
                }
                else {
                    if (ir_inst_is_unary(inst)) {
                        return ((((((std::string("    ") + (cpp_assign_target(inst))) + std::string(" = ")) + (cpp_operator(ir_inst_unary_op(inst)))) + std::string("")) + (cpp_safe_name(ir_inst_unary_operand(inst)))) + std::string(";"));
                    }
                    else {
                        if (ir_inst_is_binary(inst)) {
                            return ((((((((std::string("    ") + (cpp_assign_target(inst))) + std::string(" = ")) + (cpp_safe_name(ir_inst_binary_left(inst)))) + std::string(" ")) + (cpp_operator(ir_inst_binary_op(inst)))) + std::string(" ")) + (cpp_safe_name(ir_inst_binary_right(inst)))) + std::string(";"));
                        }
                        else {
                            if (ir_inst_is_call_like(inst)) {
                                std::vector<std::string> rendered_args = {};
                                for (const auto& arg : ir_inst_args(inst)) {
                                    rendered_args.push_back(cpp_safe_name(arg));
                                }
                                std::string call_expr = ((((std::string("") + (ir_inst_call_target_name(inst))) + std::string("(")) + (lv_join(rendered_args, std::string(", ")))) + std::string(")"));
                                std::string call_mode = ir_inst_call_mode(inst);
                                if ((call_mode == std::string("value"))) {
                                    return ((((std::string("    ") + (cpp_assign_target(inst))) + std::string(" = ")) + (call_expr)) + std::string(";"));
                                }
                                else {
                                    if ((call_mode == std::string("auto"))) {
                                        return ((((std::string("    auto ") + (cpp_safe_name(ir_inst_dest(inst)))) + std::string(" = ")) + (call_expr)) + std::string(";"));
                                    }
                                }
                                return ((std::string("    ") + (call_expr)) + std::string(";"));
                            }
                            else {
                                if (ir_inst_is_field_read(inst)) {
                                    return ((((((std::string("    ") + (cpp_assign_target(inst))) + std::string(" = ")) + (cpp_safe_name(ir_inst_field_object(inst)))) + std::string(".")) + (ir_inst_field_name(inst))) + std::string(";"));
                                }
                                else {
                                    if (ir_inst_is_field_write(inst)) {
                                        return ((((((std::string("    ") + (cpp_safe_name(ir_inst_field_object(inst)))) + std::string(".")) + (ir_inst_field_name(inst))) + std::string(" = ")) + (cpp_safe_name(ir_inst_field_value(inst)))) + std::string(";"));
                                    }
                                    else {
                                        if (ir_inst_is_index_read(inst)) {
                                            return ((((((std::string("    ") + (cpp_assign_target(inst))) + std::string(" = ")) + (cpp_safe_name(ir_inst_index_object(inst)))) + std::string("[")) + (cpp_safe_name(ir_inst_index_value(inst)))) + std::string("];"));
                                        }
                                        else {
                                            if (ir_inst_is_index_write(inst)) {
                                                return ((((((std::string("    ") + (cpp_safe_name(ir_inst_index_object(inst)))) + std::string("[")) + (cpp_safe_name(ir_inst_index_value(inst)))) + std::string("] = ")) + (cpp_safe_name(ir_inst_index_set_value(inst)))) + std::string(";"));
                                            }
                                            else {
                                                if (ir_inst_is_convert(inst)) {
                                                    if (ir_type_is_auto(ir_inst_value_type(inst))) {
                                                        return ((((std::string("    ") + (cpp_assign_target(inst))) + std::string(" = ")) + (cpp_safe_name(ir_inst_source_name(inst)))) + std::string(";"));
                                                    }
                                                    return ((((((std::string("    ") + (cpp_assign_target(inst))) + std::string(" = static_cast<")) + (cpp_type_from_ir(ir_inst_value_type(inst)))) + std::string(">(")) + (cpp_safe_name(ir_inst_source_name(inst)))) + std::string(");"));
                                                }
                                                else {
                                                    if (ir_inst_is_bind_auto(inst)) {
                                                        return ((((std::string("    auto ") + (cpp_safe_name(ir_inst_dest(inst)))) + std::string(" = ")) + (ir_inst_bind_expr(inst))) + std::string(";"));
                                                    }
                                                    else {
                                                        if (ir_inst_is_make_closure(inst)) {
                                                            return cpp_render_closure_inst(inst, closures);
                                                        }
                                                        else {
                                                            if (ir_inst_is_enum_check(inst)) {
                                                                return ((((((((std::string("    ") + (cpp_assign_target(inst))) + std::string(" = std::holds_alternative<std::decay_t<decltype(")) + (cpp_safe_name(ir_inst_enum_object(inst)))) + std::string(")>::")) + (ir_inst_enum_variant(inst))) + std::string(">(")) + (cpp_safe_name(ir_inst_enum_object(inst)))) + std::string("._data);"));
                                                            }
                                                            else {
                                                                if (ir_inst_is_enum_read(inst)) {
                                                                    return ((((((((((std::string("    ") + (cpp_assign_target(inst))) + std::string(" = std::get<std::decay_t<decltype(")) + (cpp_safe_name(ir_inst_enum_object(inst)))) + std::string(")>::")) + (ir_inst_enum_variant(inst))) + std::string(">(")) + (cpp_safe_name(ir_inst_enum_object(inst)))) + std::string("._data).")) + (ir_inst_enum_field(inst))) + std::string(";"));
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return std::string("    /* unknown inst */");
}

std::vector<std::string> cpp_render_terminator(const IrTerminator& term, bool is_entry, const std::string& return_type) {
    std::vector<std::string> lines = {};
    if (ir_term_is_jump(term)) {
        lines.push_back(((std::string("    goto ") + (ir_term_jump_target(term))) + std::string(";")));
    }
    else {
        if (ir_term_is_branch(term)) {
            lines.push_back(((((((std::string("    if (") + (cpp_safe_name(ir_term_condition(term)))) + std::string(") goto ")) + (ir_term_then_target(term))) + std::string("; else goto ")) + (ir_term_else_target(term))) + std::string(";")));
        }
        else {
            if (ir_term_is_return_void(term)) {
                if (is_entry) {
                    lines.push_back(std::string("    return 0;"));
                }
                else {
                    if ((return_type == std::string("void"))) {
                        lines.push_back(std::string("    return;"));
                    }
                    else {
                        lines.push_back(std::string("    return {};"));
                    }
                }
            }
            else {
                if (ir_term_is_return_value(term)) {
                    lines.push_back(((std::string("    return ") + (cpp_safe_name(ir_term_return_value_name(term)))) + std::string(";")));
                }
                else {
                    lines.push_back(std::string("    std::abort();"));
                }
            }
        }
    }
    return lines;
}

struct IntrinsicId {
    struct None {};
    struct Len {};
    struct Contains {};
    struct Has {};
    struct IsEmpty {};
    struct Push {};
    struct PushCopy {};
    struct Pop {};
    struct Remove {};
    struct Clear {};
    struct Sort {};
    struct Reverse {};
    struct Unique {};
    struct Flatten {};
    struct Slice {};
    struct Join {};
    struct IndexOf {};
    struct Keys {};
    struct Values {};
    struct Add {};
    struct Print {};
    struct Println {};
    struct Assert {};

    std::string _tag;
    std::variant<IntrinsicId::None, IntrinsicId::Len, IntrinsicId::Contains, IntrinsicId::Has, IntrinsicId::IsEmpty, IntrinsicId::Push, IntrinsicId::PushCopy, IntrinsicId::Pop, IntrinsicId::Remove, IntrinsicId::Clear, IntrinsicId::Sort, IntrinsicId::Reverse, IntrinsicId::Unique, IntrinsicId::Flatten, IntrinsicId::Slice, IntrinsicId::Join, IntrinsicId::IndexOf, IntrinsicId::Keys, IntrinsicId::Values, IntrinsicId::Add, IntrinsicId::Print, IntrinsicId::Println, IntrinsicId::Assert> _data;

    static IntrinsicId make_None() { return {"None", None{}}; }
    static IntrinsicId make_Len() { return {"Len", Len{}}; }
    static IntrinsicId make_Contains() { return {"Contains", Contains{}}; }
    static IntrinsicId make_Has() { return {"Has", Has{}}; }
    static IntrinsicId make_IsEmpty() { return {"IsEmpty", IsEmpty{}}; }
    static IntrinsicId make_Push() { return {"Push", Push{}}; }
    static IntrinsicId make_PushCopy() { return {"PushCopy", PushCopy{}}; }
    static IntrinsicId make_Pop() { return {"Pop", Pop{}}; }
    static IntrinsicId make_Remove() { return {"Remove", Remove{}}; }
    static IntrinsicId make_Clear() { return {"Clear", Clear{}}; }
    static IntrinsicId make_Sort() { return {"Sort", Sort{}}; }
    static IntrinsicId make_Reverse() { return {"Reverse", Reverse{}}; }
    static IntrinsicId make_Unique() { return {"Unique", Unique{}}; }
    static IntrinsicId make_Flatten() { return {"Flatten", Flatten{}}; }
    static IntrinsicId make_Slice() { return {"Slice", Slice{}}; }
    static IntrinsicId make_Join() { return {"Join", Join{}}; }
    static IntrinsicId make_IndexOf() { return {"IndexOf", IndexOf{}}; }
    static IntrinsicId make_Keys() { return {"Keys", Keys{}}; }
    static IntrinsicId make_Values() { return {"Values", Values{}}; }
    static IntrinsicId make_Add() { return {"Add", Add{}}; }
    static IntrinsicId make_Print() { return {"Print", Print{}}; }
    static IntrinsicId make_Println() { return {"Println", Println{}}; }
    static IntrinsicId make_Assert() { return {"Assert", Assert{}}; }

    std::string operator[](const std::string& key) const {
        if (key == "_tag") return _tag;
        return "";
    }
};

void print(const IntrinsicId& _v) { std::cout << "IntrinsicId(" << _v._tag << ")" << std::endl; }
std::string operator+(const std::string& _s, const IntrinsicId& _v) { return _s + _v._tag; }
std::string operator+(const IntrinsicId& _v, const std::string& _s) { return _v._tag + _s; }

std::string intrinsic_runtime_name(const IntrinsicId& intrinsic) {
    {
        const auto& _match_106 = intrinsic;
        if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Len>(_match_106._data)) {
            return std::string("len");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Contains>(_match_106._data)) {
            return std::string("lv_contains");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Has>(_match_106._data)) {
            return std::string("lv_has");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::IsEmpty>(_match_106._data)) {
            return std::string("lv_is_empty");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Push>(_match_106._data)) {
            return std::string("lv_push");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::PushCopy>(_match_106._data)) {
            return std::string("lv_push_copy");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Pop>(_match_106._data)) {
            return std::string("lv_pop");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Remove>(_match_106._data)) {
            return std::string("lv_remove");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Clear>(_match_106._data)) {
            return std::string("lv_clear");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Sort>(_match_106._data)) {
            return std::string("lv_sort");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Reverse>(_match_106._data)) {
            return std::string("lv_reverse");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Unique>(_match_106._data)) {
            return std::string("lv_unique");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Flatten>(_match_106._data)) {
            return std::string("lv_flatten");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Slice>(_match_106._data)) {
            return std::string("lv_slice");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Join>(_match_106._data)) {
            return std::string("lv_join");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::IndexOf>(_match_106._data)) {
            return std::string("lv_index_of");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Keys>(_match_106._data)) {
            return std::string("lv_keys");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Values>(_match_106._data)) {
            return std::string("lv_values");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Add>(_match_106._data)) {
            return std::string("lv_add");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Print>(_match_106._data)) {
            return std::string("print");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Println>(_match_106._data)) {
            return std::string("println");
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_106)>::Assert>(_match_106._data)) {
            return std::string("lv_assert");
        }
        else {
            return std::string("");
        }
    }
}

IntrinsicId intrinsic_from_name(std::string name) {
    if ((name == std::string("len"))) {
        return IntrinsicId::make_Len();
    }
    else {
        if ((name == std::string("lv_contains"))) {
            return IntrinsicId::make_Contains();
        }
        else {
            if ((name == std::string("lv_has"))) {
                return IntrinsicId::make_Has();
            }
            else {
                if ((name == std::string("lv_is_empty"))) {
                    return IntrinsicId::make_IsEmpty();
                }
                else {
                    if ((name == std::string("lv_push"))) {
                        return IntrinsicId::make_Push();
                    }
                    else {
                        if ((name == std::string("lv_push_copy"))) {
                            return IntrinsicId::make_PushCopy();
                        }
                        else {
                            if ((name == std::string("lv_pop"))) {
                                return IntrinsicId::make_Pop();
                            }
                            else {
                                if ((name == std::string("lv_remove"))) {
                                    return IntrinsicId::make_Remove();
                                }
                                else {
                                    if ((name == std::string("lv_clear"))) {
                                        return IntrinsicId::make_Clear();
                                    }
                                    else {
                                        if ((name == std::string("lv_sort"))) {
                                            return IntrinsicId::make_Sort();
                                        }
                                        else {
                                            if ((name == std::string("lv_reverse"))) {
                                                return IntrinsicId::make_Reverse();
                                            }
                                            else {
                                                if ((name == std::string("lv_unique"))) {
                                                    return IntrinsicId::make_Unique();
                                                }
                                                else {
                                                    if ((name == std::string("lv_flatten"))) {
                                                        return IntrinsicId::make_Flatten();
                                                    }
                                                    else {
                                                        if ((name == std::string("lv_slice"))) {
                                                            return IntrinsicId::make_Slice();
                                                        }
                                                        else {
                                                            if ((name == std::string("lv_join"))) {
                                                                return IntrinsicId::make_Join();
                                                            }
                                                            else {
                                                                if ((name == std::string("lv_index_of"))) {
                                                                    return IntrinsicId::make_IndexOf();
                                                                }
                                                                else {
                                                                    if ((name == std::string("lv_keys"))) {
                                                                        return IntrinsicId::make_Keys();
                                                                    }
                                                                    else {
                                                                        if ((name == std::string("lv_values"))) {
                                                                            return IntrinsicId::make_Values();
                                                                        }
                                                                        else {
                                                                            if ((name == std::string("lv_add"))) {
                                                                                return IntrinsicId::make_Add();
                                                                            }
                                                                            else {
                                                                                if ((name == std::string("print"))) {
                                                                                    return IntrinsicId::make_Print();
                                                                                }
                                                                                else {
                                                                                    if ((name == std::string("println"))) {
                                                                                        return IntrinsicId::make_Println();
                                                                                    }
                                                                                    else {
                                                                                        if ((name == std::string("lv_assert"))) {
                                                                                            return IntrinsicId::make_Assert();
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return IntrinsicId::make_None();
}

IntrinsicId intrinsic_collection_method_id(const TypeNode& object_type, std::string method_name) {
    {
        const auto& _match_107 = object_type;
        if (std::holds_alternative<std::decay_t<decltype(_match_107)>::Str>(_match_107._data)) {
            if ((method_name == std::string("len"))) {
                return IntrinsicId::make_Len();
            }
            else {
                if ((method_name == std::string("contains"))) {
                    return IntrinsicId::make_Contains();
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_107)>::Array>(_match_107._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_107)>::Array>(_match_107._data);
            auto& inner = *_v.inner;
            if ((method_name == std::string("len"))) {
                return IntrinsicId::make_Len();
            }
            else {
                if ((method_name == std::string("indexOf"))) {
                    return IntrinsicId::make_IndexOf();
                }
                else {
                    if ((method_name == std::string("is_empty"))) {
                        return IntrinsicId::make_IsEmpty();
                    }
                    else {
                        if ((method_name == std::string("contains"))) {
                            return IntrinsicId::make_Contains();
                        }
                        else {
                            if ((method_name == std::string("pop"))) {
                                return IntrinsicId::make_Pop();
                            }
                            else {
                                if ((method_name == std::string("unique"))) {
                                    return IntrinsicId::make_Unique();
                                }
                                else {
                                    if ((method_name == std::string("slice"))) {
                                        return IntrinsicId::make_Slice();
                                    }
                                    else {
                                        if ((method_name == std::string("join"))) {
                                            return IntrinsicId::make_Join();
                                        }
                                        else {
                                            if ((method_name == std::string("flatten"))) {
                                                return IntrinsicId::make_Flatten();
                                            }
                                            else {
                                                if ((method_name == std::string("push"))) {
                                                    return IntrinsicId::make_Push();
                                                }
                                                else {
                                                    if ((method_name == std::string("remove"))) {
                                                        return IntrinsicId::make_Remove();
                                                    }
                                                    else {
                                                        if ((method_name == std::string("clear"))) {
                                                            return IntrinsicId::make_Clear();
                                                        }
                                                        else {
                                                            if ((method_name == std::string("sort"))) {
                                                                return IntrinsicId::make_Sort();
                                                            }
                                                            else {
                                                                if ((method_name == std::string("reverse"))) {
                                                                    return IntrinsicId::make_Reverse();
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_107)>::HashMap>(_match_107._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_107)>::HashMap>(_match_107._data);
            auto& key_type = *_v.key_type;
            auto& value_type = *_v.value_type;
            if ((method_name == std::string("len"))) {
                return IntrinsicId::make_Len();
            }
            else {
                if ((method_name == std::string("has"))) {
                    return IntrinsicId::make_Has();
                }
                else {
                    if ((method_name == std::string("is_empty"))) {
                        return IntrinsicId::make_IsEmpty();
                    }
                    else {
                        if ((method_name == std::string("keys"))) {
                            return IntrinsicId::make_Keys();
                        }
                        else {
                            if ((method_name == std::string("values"))) {
                                return IntrinsicId::make_Values();
                            }
                            else {
                                if ((method_name == std::string("remove"))) {
                                    return IntrinsicId::make_Remove();
                                }
                                else {
                                    if ((method_name == std::string("clear"))) {
                                        return IntrinsicId::make_Clear();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_107)>::HashSet>(_match_107._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_107)>::HashSet>(_match_107._data);
            auto& inner = *_v.inner;
            if ((method_name == std::string("len"))) {
                return IntrinsicId::make_Len();
            }
            else {
                if ((method_name == std::string("contains"))) {
                    return IntrinsicId::make_Contains();
                }
                else {
                    if ((method_name == std::string("is_empty"))) {
                        return IntrinsicId::make_IsEmpty();
                    }
                    else {
                        if ((method_name == std::string("add"))) {
                            return IntrinsicId::make_Add();
                        }
                        else {
                            if ((method_name == std::string("remove"))) {
                                return IntrinsicId::make_Remove();
                            }
                            else {
                                if ((method_name == std::string("clear"))) {
                                    return IntrinsicId::make_Clear();
                                }
                            }
                        }
                    }
                }
            }
        }
        else {
            /* pass */
        }
    }
    return IntrinsicId::make_None();
}

TypeNode intrinsic_type_return_type(const IntrinsicId& intrinsic, const TypeNode& first_arg_type) {
    {
        const auto& _match_108 = intrinsic;
        if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Len>(_match_108._data)) {
            return TypeNode::make_Int();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::IndexOf>(_match_108._data)) {
            return TypeNode::make_Int();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Contains>(_match_108._data)) {
            return TypeNode::make_Bool();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Has>(_match_108._data)) {
            return TypeNode::make_Bool();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::IsEmpty>(_match_108._data)) {
            return TypeNode::make_Bool();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Join>(_match_108._data)) {
            return TypeNode::make_Str();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Push>(_match_108._data)) {
            return TypeNode::make_Void();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Remove>(_match_108._data)) {
            return TypeNode::make_Void();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Clear>(_match_108._data)) {
            return TypeNode::make_Void();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Sort>(_match_108._data)) {
            return TypeNode::make_Void();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Reverse>(_match_108._data)) {
            return TypeNode::make_Void();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Add>(_match_108._data)) {
            return TypeNode::make_Void();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Pop>(_match_108._data)) {
            {
                const auto& _match_109 = first_arg_type;
                if (std::holds_alternative<std::decay_t<decltype(_match_109)>::Array>(_match_109._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_109)>::Array>(_match_109._data);
                    auto& inner = *_v.inner;
                    return inner;
                }
                else {
                    return TypeNode::make_Auto();
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Unique>(_match_108._data)) {
            return first_arg_type;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Slice>(_match_108._data)) {
            return first_arg_type;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::PushCopy>(_match_108._data)) {
            return first_arg_type;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Flatten>(_match_108._data)) {
            {
                const auto& _match_110 = first_arg_type;
                if (std::holds_alternative<std::decay_t<decltype(_match_110)>::Array>(_match_110._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_110)>::Array>(_match_110._data);
                    auto& inner = *_v.inner;
                    return inner;
                }
                else {
                    return TypeNode::make_Auto();
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Keys>(_match_108._data)) {
            {
                const auto& _match_111 = first_arg_type;
                if (std::holds_alternative<std::decay_t<decltype(_match_111)>::HashMap>(_match_111._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_111)>::HashMap>(_match_111._data);
                    auto& key_type = *_v.key_type;
                    auto& value_type = *_v.value_type;
                    return TypeNode::make_Array(key_type);
                }
                else {
                    return TypeNode::make_Auto();
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_108)>::Values>(_match_108._data)) {
            {
                const auto& _match_112 = first_arg_type;
                if (std::holds_alternative<std::decay_t<decltype(_match_112)>::HashMap>(_match_112._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_112)>::HashMap>(_match_112._data);
                    auto& key_type = *_v.key_type;
                    auto& value_type = *_v.value_type;
                    return TypeNode::make_Array(value_type);
                }
                else {
                    return TypeNode::make_Auto();
                }
            }
        }
        else {
            return TypeNode::make_Auto();
        }
    }
}

TypeNode intrinsic_collection_method_return_type(const TypeNode& object_type, std::string method_name) {
    IntrinsicId intrinsic = intrinsic_collection_method_id(object_type, method_name);
    return intrinsic_type_return_type(intrinsic, object_type);
}

std::string intrinsic_collection_method_builtin_name(const TypeNode& object_type, std::string method_name) {
    IntrinsicId intrinsic = intrinsic_collection_method_id(object_type, method_name);
    return intrinsic_runtime_name(intrinsic);
}

TypeNode intrinsic_builtin_call_return_type(std::string name, const TypeNode& first_arg_type) {
    if ((name == std::string("to_string")) || (name == std::string("typeof")) || (name == std::string("input"))) {
        return TypeNode::make_Str();
    }
    else {
        if ((name == std::string("exit"))) {
            return TypeNode::make_Void();
        }
        else {
            if ((name == std::string("abs"))) {
                {
                    const auto& _match_113 = first_arg_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_113)>::Float>(_match_113._data)) {
                        return TypeNode::make_Float();
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_113)>::Float32>(_match_113._data)) {
                        return TypeNode::make_Float32();
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_113)>::Int32>(_match_113._data)) {
                        return TypeNode::make_Int32();
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_113)>::Int16>(_match_113._data)) {
                        return TypeNode::make_Int16();
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_113)>::Int8>(_match_113._data)) {
                        return TypeNode::make_Int8();
                    }
                    else {
                        return TypeNode::make_Int();
                    }
                }
            }
        }
    }
    IntrinsicId intrinsic = intrinsic_from_name(name);
    return intrinsic_type_return_type(intrinsic, first_arg_type);
}

IrType intrinsic_ir_element_type(const IrType& container_type) {
    if (container_type.name.starts_with(std::string("array[")) && container_type.name.ends_with(std::string("]"))) {
        return ir_type(container_type.name.substr(INT64_C(6), ((static_cast<int64_t>(container_type.name.size()) - INT64_C(1))) - (INT64_C(6))));
    }
    else {
        if (container_type.name.starts_with(std::string("map[")) && container_type.name.ends_with(std::string("]"))) {
            int64_t comma_pos = lv_index_of(container_type.name, std::string(", "));
            if ((comma_pos >= INT64_C(0))) {
                return ir_type(container_type.name.substr((comma_pos + INT64_C(2)), ((static_cast<int64_t>(container_type.name.size()) - INT64_C(1))) - ((comma_pos + INT64_C(2)))));
            }
        }
    }
    return ir_auto_type();
}

IrType intrinsic_ir_key_type(const IrType& container_type) {
    if (container_type.name.starts_with(std::string("map[")) && container_type.name.ends_with(std::string("]"))) {
        int64_t comma_pos = lv_index_of(container_type.name, std::string(", "));
        if ((comma_pos >= INT64_C(0))) {
            return ir_type(container_type.name.substr(INT64_C(4), (comma_pos) - (INT64_C(4))));
        }
    }
    return ir_auto_type();
}

IrType intrinsic_ir_return_type(const IntrinsicId& intrinsic, const IrType& first_arg_type) {
    {
        const auto& _match_114 = intrinsic;
        if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Len>(_match_114._data)) {
            return ir_int_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::IndexOf>(_match_114._data)) {
            return ir_int_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Contains>(_match_114._data)) {
            return ir_bool_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Has>(_match_114._data)) {
            return ir_bool_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::IsEmpty>(_match_114._data)) {
            return ir_bool_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Join>(_match_114._data)) {
            return ir_string_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Print>(_match_114._data)) {
            return ir_void_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Println>(_match_114._data)) {
            return ir_void_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Assert>(_match_114._data)) {
            return ir_void_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Push>(_match_114._data)) {
            return ir_void_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Remove>(_match_114._data)) {
            return ir_void_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Clear>(_match_114._data)) {
            return ir_void_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Sort>(_match_114._data)) {
            return ir_void_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Reverse>(_match_114._data)) {
            return ir_void_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Add>(_match_114._data)) {
            return ir_void_type();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Pop>(_match_114._data)) {
            return intrinsic_ir_element_type(first_arg_type);
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Unique>(_match_114._data)) {
            return first_arg_type;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Slice>(_match_114._data)) {
            return first_arg_type;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::PushCopy>(_match_114._data)) {
            return first_arg_type;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Flatten>(_match_114._data)) {
            return intrinsic_ir_element_type(first_arg_type);
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Keys>(_match_114._data)) {
            return ir_array_type(intrinsic_ir_key_type(first_arg_type));
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_114)>::Values>(_match_114._data)) {
            return ir_array_type(intrinsic_ir_element_type(first_arg_type));
        }
        else {
            return ir_auto_type();
        }
    }
}

IrType intrinsic_ir_builtin_return_type(std::string callee_name) {
    IntrinsicId intrinsic = intrinsic_from_name(callee_name);
    return intrinsic_ir_return_type(intrinsic, ir_auto_type());
}

IrType intrinsic_ir_container_return_type(std::string callee_name, const IrType& first_arg_type) {
    IntrinsicId intrinsic = intrinsic_from_name(callee_name);
    return intrinsic_ir_return_type(intrinsic, first_arg_type);
}

std::string stdlib_runtime_name(std::string name) {
    if ((name == std::string("map"))) {
        return std::string("__lv_col_map");
    }
    else {
        if ((name == std::string("filter"))) {
            return std::string("__lv_col_filter");
        }
        else {
            if ((name == std::string("reduce"))) {
                return std::string("__lv_col_reduce");
            }
            else {
                if ((name == std::string("for_each"))) {
                    return std::string("__lv_col_for_each");
                }
                else {
                    if ((name == std::string("zip"))) {
                        return std::string("__lv_col_zip");
                    }
                    else {
                        if ((name == std::string("take"))) {
                            return std::string("__lv_col_take");
                        }
                        else {
                            if ((name == std::string("drop"))) {
                                return std::string("__lv_col_drop");
                            }
                            else {
                                if ((name == std::string("enumerate"))) {
                                    return std::string("__lv_col_enumerate");
                                }
                                else {
                                    if ((name == std::string("range"))) {
                                        return std::string("__lv_col_range");
                                    }
                                    else {
                                        if ((name == std::string("range_step"))) {
                                            return std::string("__lv_col_range_step");
                                        }
                                        else {
                                            if ((name == std::string("set_union"))) {
                                                return std::string("__lv_col_set_union");
                                            }
                                            else {
                                                if ((name == std::string("set_intersection"))) {
                                                    return std::string("__lv_col_set_intersection");
                                                }
                                                else {
                                                    if ((name == std::string("set_difference"))) {
                                                        return std::string("__lv_col_set_difference");
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return std::string("");
}

std::string stdlib_collection_method_runtime_name(const TypeNode& object_type, std::string method_name) {
    {
        const auto& _match_115 = object_type;
        if (std::holds_alternative<std::decay_t<decltype(_match_115)>::Array>(_match_115._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_115)>::Array>(_match_115._data);
            auto& inner = *_v.inner;
            if ((method_name == std::string("map"))) {
                return std::string("__lv_col_map");
            }
            else {
                if ((method_name == std::string("filter"))) {
                    return std::string("__lv_col_filter");
                }
                else {
                    if ((method_name == std::string("reduce"))) {
                        return std::string("__lv_col_reduce");
                    }
                    else {
                        if ((method_name == std::string("for_each"))) {
                            return std::string("__lv_col_for_each");
                        }
                        else {
                            if ((method_name == std::string("zip"))) {
                                return std::string("__lv_col_zip");
                            }
                            else {
                                if ((method_name == std::string("take"))) {
                                    return std::string("__lv_col_take");
                                }
                                else {
                                    if ((method_name == std::string("drop"))) {
                                        return std::string("__lv_col_drop");
                                    }
                                    else {
                                        if ((method_name == std::string("enumerate"))) {
                                            return std::string("__lv_col_enumerate");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_115)>::HashSet>(_match_115._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_115)>::HashSet>(_match_115._data);
            auto& inner = *_v.inner;
            if ((method_name == std::string("union_with"))) {
                return std::string("__lv_col_set_union");
            }
            else {
                if ((method_name == std::string("intersect"))) {
                    return std::string("__lv_col_set_intersection");
                }
                else {
                    if ((method_name == std::string("difference"))) {
                        return std::string("__lv_col_set_difference");
                    }
                }
            }
        }
        else {
            /* pass */
        }
    }
    return std::string("");
}

TypeNode pair_field_type(const TypeNode& pair_type, std::string field_name) {
    {
        const auto& _match_116 = pair_type;
        if (std::holds_alternative<std::decay_t<decltype(_match_116)>::Custom>(_match_116._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_116)>::Custom>(_match_116._data);
            auto& name = _v.name;
            auto& type_args = _v.type_args;
            if ((name == std::string("std::pair")) && (static_cast<int64_t>(type_args.size()) >= INT64_C(2))) {
                if ((field_name == std::string("first"))) {
                    return type_args[INT64_C(0)];
                }
                else {
                    if ((field_name == std::string("second"))) {
                        return type_args[INT64_C(1)];
                    }
                }
            }
        }
        else {
            /* pass */
        }
    }
    return TypeNode::make_Auto();
}

struct TypedCallTargetKind {
    struct None {};
    struct DirectFunction {};
    struct Intrinsic {};
    struct DirectConstructor {};
    struct ClosureValue {};
    struct Indirect {};

    std::string _tag;
    std::variant<TypedCallTargetKind::None, TypedCallTargetKind::DirectFunction, TypedCallTargetKind::Intrinsic, TypedCallTargetKind::DirectConstructor, TypedCallTargetKind::ClosureValue, TypedCallTargetKind::Indirect> _data;

    static TypedCallTargetKind make_None() { return {"None", None{}}; }
    static TypedCallTargetKind make_DirectFunction() { return {"DirectFunction", DirectFunction{}}; }
    static TypedCallTargetKind make_Intrinsic() { return {"Intrinsic", Intrinsic{}}; }
    static TypedCallTargetKind make_DirectConstructor() { return {"DirectConstructor", DirectConstructor{}}; }
    static TypedCallTargetKind make_ClosureValue() { return {"ClosureValue", ClosureValue{}}; }
    static TypedCallTargetKind make_Indirect() { return {"Indirect", Indirect{}}; }

    std::string operator[](const std::string& key) const {
        if (key == "_tag") return _tag;
        return "";
    }
};

void print(const TypedCallTargetKind& _v) { std::cout << "TypedCallTargetKind(" << _v._tag << ")" << std::endl; }
std::string operator+(const std::string& _s, const TypedCallTargetKind& _v) { return _s + _v._tag; }
std::string operator+(const TypedCallTargetKind& _v, const std::string& _s) { return _v._tag + _s; }

struct TypedCallTarget {
    TypedCallTargetKind kind;
    std::string source_name;
    std::string lowered_name;
    IntrinsicId intrinsic;

};

struct TypedCallSig {
    std::vector<TypeNode> param_types;
    TypeNode return_type;

};

struct TypedClosureSig {
    std::vector<TypeNode> capture_types;
    std::vector<TypeNode> param_types;
    TypeNode return_type;

};

TypedCallTarget typed_call_target_none() {
    return TypedCallTarget(TypedCallTargetKind::make_None(), std::string(""), std::string(""), IntrinsicId::make_None());
}

TypedCallSig typed_call_sig(const std::vector<TypeNode>& param_types, const TypeNode& return_type) {
    return TypedCallSig(param_types, return_type);
}

TypedCallSig typed_call_sig_none() {
    std::vector<TypeNode> empty = {};
    return TypedCallSig(empty, TypeNode::make_Void());
}

TypedClosureSig typed_closure_sig(const std::vector<TypeNode>& capture_types, const std::vector<TypeNode>& param_types, const TypeNode& return_type) {
    return TypedClosureSig(capture_types, param_types, return_type);
}

TypedClosureSig typed_closure_sig_none() {
    std::vector<TypeNode> empty = {};
    return TypedClosureSig(empty, empty, TypeNode::make_Void());
}

TypeNode typed_closure_value_type(const TypedClosureSig& signature) {
    std::vector<TypeNode> parts = {};
    for (const auto& param_type : signature.param_types) {
        parts.push_back(param_type);
    }
    parts.push_back(signature.return_type);
    return TypeNode::make_Custom(std::string("__closure"), parts);
}

bool typed_is_closure_type(const TypeNode& value_type) {
    {
        const auto& _match_117 = value_type;
        if (std::holds_alternative<std::decay_t<decltype(_match_117)>::Custom>(_match_117._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_117)>::Custom>(_match_117._data);
            auto& name = _v.name;
            auto& type_args = _v.type_args;
            return (name == std::string("__closure"));
        }
        else {
            return false;
        }
    }
}

std::vector<TypeNode> typed_closure_param_types(const TypeNode& value_type) {
    {
        const auto& _match_118 = value_type;
        if (std::holds_alternative<std::decay_t<decltype(_match_118)>::Custom>(_match_118._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_118)>::Custom>(_match_118._data);
            auto& name = _v.name;
            auto& type_args = _v.type_args;
            if ((name == std::string("__closure")) && (static_cast<int64_t>(type_args.size()) > INT64_C(0))) {
                std::vector<TypeNode> params = {};
                for (int64_t i = INT64_C(0); i < (static_cast<int64_t>(type_args.size()) - INT64_C(1)); i++) {
                    params.push_back(type_args[i]);
                }
                return params;
            }
        }
        else {
            /* pass */
        }
    }
    return {};
}

TypeNode typed_closure_return_type(const TypeNode& value_type) {
    {
        const auto& _match_119 = value_type;
        if (std::holds_alternative<std::decay_t<decltype(_match_119)>::Custom>(_match_119._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_119)>::Custom>(_match_119._data);
            auto& name = _v.name;
            auto& type_args = _v.type_args;
            if ((name == std::string("__closure")) && (static_cast<int64_t>(type_args.size()) > INT64_C(0))) {
                return type_args[(static_cast<int64_t>(type_args.size()) - INT64_C(1))];
            }
        }
        else {
            /* pass */
        }
    }
    return TypeNode::make_Auto();
}

TypedCallTarget typed_call_target_function(std::string name) {
    return TypedCallTarget(TypedCallTargetKind::make_DirectFunction(), name, name, IntrinsicId::make_None());
}

TypedCallTarget typed_call_target_intrinsic(const IntrinsicId& intrinsic) {
    std::string name = intrinsic_runtime_name(intrinsic);
    return TypedCallTarget(TypedCallTargetKind::make_Intrinsic(), name, name, intrinsic);
}

TypedCallTarget typed_call_target_constructor(std::string name) {
    return TypedCallTarget(TypedCallTargetKind::make_DirectConstructor(), name, name, IntrinsicId::make_None());
}

TypedCallTarget typed_call_target_closure(std::string name) {
    return TypedCallTarget(TypedCallTargetKind::make_ClosureValue(), name, name, IntrinsicId::make_None());
}

TypedCallTarget typed_call_target_indirect(std::string name) {
    return TypedCallTarget(TypedCallTargetKind::make_Indirect(), name, name, IntrinsicId::make_None());
}

std::string typed_method_name(std::string struct_name, std::string method_name) {
    return ((((std::string("") + (struct_name)) + std::string("__")) + (method_name)) + std::string(""));
}

bool typed_call_is_intrinsic(const TypedCallTarget& target) {
    {
        const auto& _match_120 = target.kind;
        if (std::holds_alternative<std::decay_t<decltype(_match_120)>::Intrinsic>(_match_120._data)) {
            return true;
        }
        else {
            return false;
        }
    }
}

bool typed_call_is_function_like(const TypedCallTarget& target) {
    {
        const auto& _match_121 = target.kind;
        if (std::holds_alternative<std::decay_t<decltype(_match_121)>::DirectFunction>(_match_121._data)) {
            return true;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_121)>::DirectConstructor>(_match_121._data)) {
            return true;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_121)>::ClosureValue>(_match_121._data)) {
            return true;
        }
        else {
            return false;
        }
    }
}

TypedCallTarget typed_call_target_from_surface(const Checker& checker, const Expr& callee, const TypeNode& object_type, const TypeNode& callee_type) {
    {
        const auto& _match_122 = callee;
        if (std::holds_alternative<std::decay_t<decltype(_match_122)>::Variable>(_match_122._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_122)>::Variable>(_match_122._data);
            auto& name = _v.name;
            std::string stdlib_name = stdlib_runtime_name(name.lexeme);
            if ((stdlib_name != std::string(""))) {
                return typed_call_target_function(stdlib_name);
            }
            IntrinsicId intrinsic = intrinsic_from_name(name.lexeme);
            {
                const auto& _match_123 = intrinsic;
                if (_match_123._tag == "None") {
                    /* pass */
                }
                else {
                    return typed_call_target_intrinsic(intrinsic);
                }
            }
            if ((checker.known_funcs.count(name.lexeme) > 0)) {
                return typed_call_target_function(name.lexeme);
            }
            if ((checker.known_classes.count(name.lexeme) > 0)) {
                return typed_call_target_constructor(name.lexeme);
            }
            if (typed_is_closure_type(callee_type)) {
                return typed_call_target_closure(name.lexeme);
            }
            return typed_call_target_indirect(name.lexeme);
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_122)>::Get>(_match_122._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_122)>::Get>(_match_122._data);
            auto& object = *_v.object;
            auto& name = _v.name;
            IntrinsicId collection_intrinsic = intrinsic_collection_method_id(object_type, name.lexeme);
            {
                const auto& _match_124 = collection_intrinsic;
                if (_match_124._tag == "None") {
                    /* pass */
                }
                else {
                    return typed_call_target_intrinsic(collection_intrinsic);
                }
            }
            std::string stdlib_name = stdlib_collection_method_runtime_name(object_type, name.lexeme);
            if ((stdlib_name != std::string(""))) {
                return typed_call_target_function(stdlib_name);
            }
            {
                const auto& _match_125 = object_type;
                if (std::holds_alternative<std::decay_t<decltype(_match_125)>::Custom>(_match_125._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_125)>::Custom>(_match_125._data);
                    auto& struct_name = _v.name;
                    auto& type_args = _v.type_args;
                    if ((checker.known_classes.count(struct_name) > 0)) {
                        return typed_call_target_function(typed_method_name(struct_name, name.lexeme));
                    }
                    if ((checker.known_enums.count(struct_name) > 0)) {
                        return typed_call_target_function(typed_method_name(struct_name, name.lexeme));
                    }
                }
                else {
                    /* pass */
                }
            }
            return typed_call_target_indirect(name.lexeme);
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_122)>::StaticGet>(_match_122._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_122)>::StaticGet>(_match_122._data);
            auto& object = *_v.object;
            auto& name = _v.name;
            {
                const auto& _match_126 = object;
                if (std::holds_alternative<std::decay_t<decltype(_match_126)>::Variable>(_match_126._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_126)>::Variable>(_match_126._data);
                    auto& obj_name = _v.name;
                    std::string stdlib_name = stdlib_runtime_name(name.lexeme);
                    if ((stdlib_name != std::string(""))) {
                        return typed_call_target_function(stdlib_name);
                    }
                    if ((checker.known_enums.count(obj_name.lexeme) > 0)) {
                        return typed_call_target_function(((((std::string("") + (obj_name.lexeme)) + std::string("::make_")) + (name.lexeme)) + std::string("")));
                    }
                }
                else {
                    /* pass */
                }
            }
            return typed_call_target_indirect(name.lexeme);
        }
        else {
            return typed_call_target_indirect(std::string(""));
        }
    }
}

TypedCallTarget typed_call_target_from_normalized_callee(const Expr& callee) {
    {
        const auto& _match_127 = callee;
        if (std::holds_alternative<std::decay_t<decltype(_match_127)>::Variable>(_match_127._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_127)>::Variable>(_match_127._data);
            auto& name = _v.name;
            IntrinsicId intrinsic = intrinsic_from_name(name.lexeme);
            {
                const auto& _match_128 = intrinsic;
                if (_match_128._tag == "None") {
                    /* pass */
                }
                else {
                    return typed_call_target_intrinsic(intrinsic);
                }
            }
            if (lv_contains(name.lexeme, std::string("::"))) {
                return typed_call_target_function(name.lexeme);
            }
            if (lv_contains(name.lexeme, std::string("__"))) {
                return typed_call_target_function(name.lexeme);
            }
            return typed_call_target_function(name.lexeme);
        }
        else {
            return typed_call_target_indirect(std::string(""));
        }
    }
}

Expr typed_call_expr(const TypedCallTarget& target, const Token& paren, const std::vector<Expr>& args) {
    std::vector<std::string> empty_arg_names = {};
    Token callee_name = Token(TK_IDENTIFIER, target.lowered_name, paren.line, paren.col);
    return Expr::make_Call(Expr::make_Variable(callee_name), paren, args, empty_arg_names);
}

struct TypedExpr {
    std::string kind;
    TypeNode value_type;
    TypedCallTarget call_target;
    TypedCallSig call_signature;
    TypedClosureSig closure_signature;
    int64_t int_a;
    std::vector<TypedExpr> children;
    std::vector<TypedExpr> extra_children;
    std::vector<std::string> names;
    std::vector<TypeNode> types;
    std::vector<std::string> capture_names;
    std::vector<TypeNode> capture_types;
    std::string text_a;
    std::string text_b;

};

TypeNode typed_expr_type(const TypedExpr& expr) {
    return expr.value_type;
}

TypedExpr typed_expr_none() {
    std::vector<TypedExpr> empty = {};
    std::vector<std::string> empty_names = {};
    std::vector<TypeNode> empty_types = {};
    return TypedExpr(std::string("none"), TypeNode::make_None(), typed_call_target_none(), typed_call_sig_none(), typed_closure_sig_none(), INT64_C(0), empty, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
}

struct ItemId {
    int64_t value;

};

ItemId item_id(int64_t value) {
    return ItemId(value);
}

struct BodyId {
    int64_t value;

};

BodyId body_id(int64_t value) {
    return BodyId(value);
}

struct TypedStmt {
    std::string kind;
    Stmt node;
    TypeNode value_type;
    std::string text_a;
    std::string text_b;
    std::vector<std::string> names;
    std::vector<std::vector<std::string>> name_groups;
    std::vector<TypedExpr> exprs;
    std::vector<TypedStmt> children;

};

TypedStmt typed_stmt_none() {
    std::vector<std::string> empty_names = {};
    std::vector<std::vector<std::string>> empty_name_groups = {};
    std::vector<TypedExpr> empty_exprs = {};
    std::vector<TypedStmt> empty_children = {};
    return TypedStmt(std::string("none"), Stmt::make_None(), TypeNode::make_None(), std::string(""), std::string(""), empty_names, empty_name_groups, empty_exprs, empty_children);
}

struct TypedFunctionInfo {
    ItemId item_id;
    BodyId body_id;
    std::string name;
    std::vector<Param> params;
    TypeNode return_type;
    std::vector<TypedStmt> body;

};

struct TypedStructField {
    std::string name;
    TypeNode field_type;

};

struct TypedStructInfo {
    ItemId item_id;
    BodyId constructor_body_id;
    std::string name;
    std::vector<TypedStructField> fields;
    std::vector<Param> constructor_params;
    std::vector<Expr> constructor_defaults;
    std::vector<TypedStmt> constructor_body;

};

struct TypedEnumVariant {
    std::string name;
    std::vector<TypeNode> types;
    std::vector<std::string> field_names;

};

struct TypedEnumInfo {
    ItemId item_id;
    std::string name;
    std::vector<TypedEnumVariant> variants;

};

struct TypedLambdaBodyInfo {
    BodyId body_id;
    std::vector<TypedStmt> body;

};

struct TypedProgram {
    std::string name;
    BodyId top_level_body_id;
    std::vector<TypedStmt> top_level;
    std::vector<TypedFunctionInfo> functions;
    std::vector<TypedStructInfo> structs;
    std::vector<TypedEnumInfo> enums;
    std::vector<TypedLambdaBodyInfo> lambda_blocks;

};

CppFunction cpp_lower_function(const IrFunction& fn_ir, const std::vector<IrClosureDef>& closures) {
    std::vector<CppParam> params = {};
    std::vector<CppLocal> locals = {};
    std::vector<std::string> body_lines = {};
    auto helper_closure = cpp_find_closure_by_helper(closures, ir_function_name(fn_ir));
    auto fn_params = ir_function_params(fn_ir);
    auto closure_capture_params = ir_closure_capture_params(helper_closure);
    for (const auto& param : fn_params) {
        std::string param_type = cpp_type_from_ir(param.param_type);
        int64_t capture_index = (-INT64_C(1));
        for (int64_t i = INT64_C(0); i < static_cast<int64_t>(closure_capture_params.size()); i++) {
            if ((i < static_cast<int64_t>(fn_params.size())) && (closure_capture_params[i].name == param.name)) {
                capture_index = i;
            }
        }
        if ((capture_index >= INT64_C(0))) {
            param_type = ((std::string("") + (param_type)) + std::string("&"));
        }
        if ((param.name == std::string("self"))) {
            param_type = ((std::string("") + (param_type)) + std::string("&"));
        }
        params.push_back(CppParam(cpp_safe_name(param.name), param_type));
    }
    for (const auto& local : ir_function_locals(fn_ir)) {
        bool is_param = false;
        for (const auto& param : fn_params) {
            if ((param.name == local.name)) {
                is_param = true;
            }
        }
        if ((!is_param)) {
            std::string local_type = cpp_type_from_ir(local.local_type);
            if ((local_type == std::string("auto"))) {
                continue;
            }
            locals.push_back(CppLocal(cpp_safe_name(local.name), local_type));
        }
    }
    for (const auto& block : ir_function_blocks(fn_ir)) {
        cpp_push_line(body_lines, ((std::string("  ") + (ir_block_name(block))) + std::string(":")));
        for (const auto& inst : ir_block_instructions(block)) {
            cpp_push_line(body_lines, cpp_render_inst(inst, closures));
        }
        auto term_lines = cpp_render_terminator(ir_block_terminator(block), ir_function_is_entry(fn_ir), cpp_type_from_ir(ir_function_return_type(fn_ir)));
        for (const auto& line : term_lines) {
            cpp_push_line(body_lines, line);
        }
    }
    std::string return_type = cpp_type_from_ir(ir_function_return_type(fn_ir));
    std::string fn_name = ir_function_name(fn_ir);
    if (ir_function_is_entry(fn_ir)) {
        return_type = std::string("int");
        fn_name = std::string("main");
    }
    return CppFunction(fn_name, return_type, params, locals, body_lines, ir_function_is_entry(fn_ir));
}

std::string cpp_expr_literal(const Expr& expr) {
    {
        const auto& _match_129 = expr;
        if (std::holds_alternative<std::decay_t<decltype(_match_129)>::Literal>(_match_129._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_129)>::Literal>(_match_129._data);
            auto& kind = _v.kind;
            auto& value = _v.value;
            if ((kind == std::string("string"))) {
                return ((std::string("\"") + (cpp_escape_string(value))) + std::string("\""));
            }
            return value;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_129)>::Unary>(_match_129._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_129)>::Unary>(_match_129._data);
            auto& op = _v.op;
            auto& right = *_v.right;
            return ((((std::string("") + (cpp_operator(op.lexeme))) + std::string("")) + (cpp_expr_literal(right))) + std::string(""));
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_129)>::Variable>(_match_129._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_129)>::Variable>(_match_129._data);
            auto& name = _v.name;
            return name.lexeme;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_129)>::Vector>(_match_129._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_129)>::Vector>(_match_129._data);
            auto& elements = _v.elements;
            std::vector<std::string> rendered = {};
            for (const auto& el : elements) {
                rendered.push_back(cpp_expr_literal(el));
            }
            return ((std::string("{") + (lv_join(rendered, std::string(", ")))) + std::string("}"));
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_129)>::Call>(_match_129._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_129)>::Call>(_match_129._data);
            auto& callee = *_v.callee;
            auto& paren = _v.paren;
            auto& args = _v.args;
            auto& arg_names = _v.arg_names;
            std::vector<std::string> rendered_args = {};
            for (const auto& arg : args) {
                rendered_args.push_back(cpp_expr_literal(arg));
            }
            {
                const auto& _match_130 = callee;
                if (std::holds_alternative<std::decay_t<decltype(_match_130)>::Variable>(_match_130._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_130)>::Variable>(_match_130._data);
                    auto& name = _v.name;
                    return ((((std::string("") + (name.lexeme)) + std::string("(")) + (lv_join(rendered_args, std::string(", ")))) + std::string(")"));
                }
                else {
                    return std::string("{}");
                }
            }
        }
        else {
            return std::string("{}");
        }
    }
}

std::string cpp_typed_expr_literal(const TypedExpr& expr) {
    if ((expr.kind == std::string("literal"))) {
        if ((expr.text_a == std::string("string"))) {
            return ((std::string("\"") + (cpp_escape_string(expr.text_b))) + std::string("\""));
        }
        return expr.text_b;
    }
    else {
        if ((expr.kind == std::string("unary"))) {
            return ((((std::string("") + (cpp_operator(expr.text_a))) + std::string("")) + (cpp_typed_expr_literal(expr.children[INT64_C(0)]))) + std::string(""));
        }
        else {
            if ((expr.kind == std::string("variable"))) {
                return expr.text_a;
            }
            else {
                if ((expr.kind == std::string("vector"))) {
                    std::vector<std::string> rendered = {};
                    for (const auto& el : expr.children) {
                        rendered.push_back(cpp_typed_expr_literal(el));
                    }
                    return ((std::string("{") + (lv_join(rendered, std::string(", ")))) + std::string("}"));
                }
                else {
                    if ((expr.kind == std::string("call"))) {
                        std::vector<std::string> rendered_args = {};
                        for (const auto& arg : expr.children) {
                            rendered_args.push_back(cpp_typed_expr_literal(arg));
                        }
                        return ((((std::string("") + (expr.call_target.lowered_name)) + std::string("(")) + (lv_join(rendered_args, std::string(", ")))) + std::string(")"));
                    }
                }
            }
        }
    }
    return std::string("{}");
}

std::string cpp_indent(int64_t level) {
    std::string out = std::string("");
    for (int64_t i = INT64_C(0); i < level; i++) {
        out = (out + std::string("    "));
    }
    return out;
}

struct TypedLambdaCppRenderer {
    std::vector<TypedLambdaBodyInfo> lambda_blocks;

    TypedLambdaCppRenderer(const std::vector<TypedLambdaBodyInfo>& lambda_blocks)
        : lambda_blocks(lambda_blocks) {
    }

    std::string render_param(const Param& param) {
        std::string cpp_type = cpp_type_from_type_node(param.param_type);
        if (param.is_mut && param.is_ref) {
            return ((((std::string("") + (cpp_type)) + std::string("& ")) + (param.name.lexeme)) + std::string(""));
        }
        else {
            if (param.is_ref) {
                return ((((std::string("const ") + (cpp_type)) + std::string("& ")) + (param.name.lexeme)) + std::string(""));
            }
        }
        return ((((std::string("") + (cpp_type)) + std::string(" ")) + (param.name.lexeme)) + std::string(""));
    }

    std::string render_typed_param(std::string name, const TypeNode& param_type) {
        std::string cpp_type = cpp_type_from_type_node(param_type);
        return ((((std::string("") + (cpp_type)) + std::string(" ")) + (name)) + std::string(""));
    }

    std::string render_capture_list(const TypedExpr& expr) {
        if ((static_cast<int64_t>(expr.capture_names.size()) == INT64_C(0))) {
            return std::string("[]");
        }
        std::vector<std::string> parts = {};
        for (const auto& capture_name : expr.capture_names) {
            parts.push_back(((std::string("&") + (capture_name)) + std::string("")));
        }
        return ((std::string("[") + (lv_join(parts, std::string(", ")))) + std::string("]"));
    }

    std::string render_expr(const TypedExpr& expr) {
        if ((expr.kind == std::string("literal"))) {
            return cpp_typed_expr_literal(expr);
        }
        else {
            if ((expr.kind == std::string("variable"))) {
                return expr.text_a;
            }
            else {
                if ((expr.kind == std::string("grouping"))) {
                    return ((std::string("(") + ((*this).render_expr(expr.children[INT64_C(0)]))) + std::string(")"));
                }
                else {
                    if ((expr.kind == std::string("unary"))) {
                        return ((((std::string("") + (cpp_operator(expr.text_a))) + std::string("")) + ((*this).render_expr(expr.children[INT64_C(0)]))) + std::string(""));
                    }
                    else {
                        if ((expr.kind == std::string("binary")) || (expr.kind == std::string("logical"))) {
                            std::string left = (*this).render_expr(expr.children[INT64_C(0)]);
                            std::string right = (*this).render_expr(expr.children[INT64_C(1)]);
                            return ((((((std::string("(") + (left)) + std::string(" ")) + (cpp_operator(expr.text_a))) + std::string(" ")) + (right)) + std::string(")"));
                        }
                        else {
                            if ((expr.kind == std::string("assign"))) {
                                return ((((std::string("") + (expr.text_a)) + std::string(" = ")) + ((*this).render_expr(expr.children[INT64_C(0)]))) + std::string(""));
                            }
                            else {
                                if ((expr.kind == std::string("call"))) {
                                    std::vector<std::string> rendered_args = {};
                                    for (const auto& arg : expr.children) {
                                        rendered_args.push_back((*this).render_expr(arg));
                                    }
                                    return ((((std::string("") + (expr.call_target.lowered_name)) + std::string("(")) + (lv_join(rendered_args, std::string(", ")))) + std::string(")"));
                                }
                                else {
                                    if ((expr.kind == std::string("get"))) {
                                        return ((((std::string("") + ((*this).render_expr(expr.children[INT64_C(0)]))) + std::string(".")) + (expr.text_a)) + std::string(""));
                                    }
                                    else {
                                        if ((expr.kind == std::string("set"))) {
                                            return ((((((std::string("") + ((*this).render_expr(expr.children[INT64_C(0)]))) + std::string(".")) + (expr.text_a)) + std::string(" = ")) + ((*this).render_expr(expr.children[INT64_C(1)]))) + std::string(""));
                                        }
                                        else {
                                            if ((expr.kind == std::string("index"))) {
                                                return ((((std::string("") + ((*this).render_expr(expr.children[INT64_C(0)]))) + std::string("[")) + ((*this).render_expr(expr.children[INT64_C(1)]))) + std::string("]"));
                                            }
                                            else {
                                                if ((expr.kind == std::string("index_set"))) {
                                                    return ((((((std::string("") + ((*this).render_expr(expr.children[INT64_C(0)]))) + std::string("[")) + ((*this).render_expr(expr.children[INT64_C(1)]))) + std::string("] = ")) + ((*this).render_expr(expr.children[INT64_C(2)]))) + std::string(""));
                                                }
                                                else {
                                                    if ((expr.kind == std::string("cast"))) {
                                                        return ((((std::string("static_cast<") + (cpp_type_from_type_node(expr.value_type))) + std::string(">(")) + ((*this).render_expr(expr.children[INT64_C(0)]))) + std::string(")"));
                                                    }
                                                    else {
                                                        if ((expr.kind == std::string("vector"))) {
                                                            std::vector<std::string> parts = {};
                                                            for (const auto& child : expr.children) {
                                                                parts.push_back((*this).render_expr(child));
                                                            }
                                                            return ((std::string("{") + (lv_join(parts, std::string(", ")))) + std::string("}"));
                                                        }
                                                        else {
                                                            if ((expr.kind == std::string("map"))) {
                                                                std::vector<std::string> parts = {};
                                                                for (int64_t i = INT64_C(0); i < static_cast<int64_t>(expr.children.size()); i++) {
                                                                    parts.push_back(((((std::string("{") + ((*this).render_expr(expr.children[i]))) + std::string(", ")) + ((*this).render_expr(expr.extra_children[i]))) + std::string("}")));
                                                                }
                                                                return ((std::string("{") + (lv_join(parts, std::string(", ")))) + std::string("}"));
                                                            }
                                                            else {
                                                                if ((expr.kind == std::string("lambda"))) {
                                                                    std::vector<std::string> param_parts = {};
                                                                    for (int64_t i = INT64_C(0); i < static_cast<int64_t>(expr.names.size()); i++) {
                                                                        if ((i < static_cast<int64_t>(expr.types.size()))) {
                                                                            param_parts.push_back((*this).render_typed_param(expr.names[i], expr.types[i]));
                                                                        }
                                                                    }
                                                                    std::string body_expr = std::string("{}");
                                                                    if ((static_cast<int64_t>(expr.children.size()) > INT64_C(0))) {
                                                                        body_expr = (*this).render_expr(expr.children[INT64_C(0)]);
                                                                    }
                                                                    return ((((((std::string("") + ((*this).render_capture_list(expr))) + std::string("(")) + (lv_join(param_parts, std::string(", ")))) + std::string(") { return ")) + (body_expr)) + std::string("; }"));
                                                                }
                                                                else {
                                                                    if ((expr.kind == std::string("block_lambda"))) {
                                                                        int64_t body_id = expr.int_a;
                                                                        if ((body_id >= INT64_C(0)) && (body_id < static_cast<int64_t>(this->lambda_blocks.size()))) {
                                                                            std::vector<std::string> param_parts = {};
                                                                            for (int64_t i = INT64_C(0); i < static_cast<int64_t>(expr.names.size()); i++) {
                                                                                if ((i < static_cast<int64_t>(expr.types.size()))) {
                                                                                    param_parts.push_back((*this).render_typed_param(expr.names[i], expr.types[i]));
                                                                                }
                                                                            }
                                                                            std::string out = ((((std::string("") + ((*this).render_capture_list(expr))) + std::string("(")) + (lv_join(param_parts, std::string(", ")))) + std::string(") {\n"));
                                                                            for (const auto& stmt : this->lambda_blocks[body_id].body) {
                                                                                out = (out + (*this).render_stmt(stmt, INT64_C(1)));
                                                                            }
                                                                            out = (out + std::string("}"));
                                                                            return out;
                                                                        }
                                                                        return ((std::string("") + ((*this).render_capture_list(expr))) + std::string("() {}"));
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return std::string("{}");
    }

    std::string render_stmt(const TypedStmt& stmt, int64_t indent_level) {
        std::string indent = cpp_indent(indent_level);
        if ((stmt.kind == std::string("expr_stmt"))) {
            if ((static_cast<int64_t>(stmt.exprs.size()) == INT64_C(0))) {
                return std::string("");
            }
            return ((((std::string("") + (indent)) + std::string("")) + ((*this).render_expr(stmt.exprs[INT64_C(0)]))) + std::string(";\n"));
        }
        else {
            if ((stmt.kind == std::string("let"))) {
                std::string declared_type = cpp_type_from_type_node(stmt.value_type);
                if ((static_cast<int64_t>(stmt.exprs.size()) == INT64_C(0)) || (stmt.exprs[INT64_C(0)].kind == std::string("none"))) {
                    return ((((((std::string("") + (indent)) + std::string("")) + (declared_type)) + std::string(" ")) + (stmt.text_a)) + std::string(";\n"));
                }
                return ((((((((std::string("") + (indent)) + std::string("")) + (declared_type)) + std::string(" ")) + (stmt.text_a)) + std::string(" = ")) + ((*this).render_expr(stmt.exprs[INT64_C(0)]))) + std::string(";\n"));
            }
            else {
                if ((stmt.kind == std::string("const"))) {
                    std::string declared_type = cpp_type_from_type_node(stmt.value_type);
                    if ((static_cast<int64_t>(stmt.exprs.size()) == INT64_C(0)) || (stmt.exprs[INT64_C(0)].kind == std::string("none"))) {
                        return ((((((std::string("") + (indent)) + std::string("const ")) + (declared_type)) + std::string(" ")) + (stmt.text_a)) + std::string(";\n"));
                    }
                    return ((((((((std::string("") + (indent)) + std::string("const ")) + (declared_type)) + std::string(" ")) + (stmt.text_a)) + std::string(" = ")) + ((*this).render_expr(stmt.exprs[INT64_C(0)]))) + std::string(";\n"));
                }
                else {
                    if ((stmt.kind == std::string("return"))) {
                        if ((static_cast<int64_t>(stmt.exprs.size()) == INT64_C(0)) || (stmt.exprs[INT64_C(0)].kind == std::string("none"))) {
                            return ((std::string("") + (indent)) + std::string("return;\n"));
                        }
                        return ((((std::string("") + (indent)) + std::string("return ")) + ((*this).render_expr(stmt.exprs[INT64_C(0)]))) + std::string(";\n"));
                    }
                    else {
                        if ((stmt.kind == std::string("block"))) {
                            std::string out = ((std::string("") + (indent)) + std::string("{\n"));
                            for (const auto& child : stmt.children) {
                                out = (out + (*this).render_stmt(child, (indent_level + INT64_C(1))));
                            }
                            out = (out + ((std::string("") + (indent)) + std::string("}\n")));
                            return out;
                        }
                        else {
                            if ((stmt.kind == std::string("if"))) {
                                std::string out = ((((std::string("") + (indent)) + std::string("if (")) + ((*this).render_expr(stmt.exprs[INT64_C(0)]))) + std::string(") "));
                                if ((static_cast<int64_t>(stmt.children.size()) > INT64_C(0)) && (stmt.children[INT64_C(0)].kind == std::string("block"))) {
                                    out = (out + (*this).render_stmt(stmt.children[INT64_C(0)], indent_level));
                                }
                                else {
                                    out = (out + std::string("{\n"));
                                    if ((static_cast<int64_t>(stmt.children.size()) > INT64_C(0))) {
                                        out = (out + (*this).render_stmt(stmt.children[INT64_C(0)], (indent_level + INT64_C(1))));
                                    }
                                    out = (out + ((std::string("") + (indent)) + std::string("}\n")));
                                }
                                if ((static_cast<int64_t>(stmt.children.size()) > INT64_C(1)) && (stmt.children[INT64_C(1)].kind != std::string("none"))) {
                                    out = out.substr(INT64_C(0), ((static_cast<int64_t>(out.size()) - INT64_C(1))) - (INT64_C(0)));
                                    out = (out + std::string(" else "));
                                    if ((stmt.children[INT64_C(1)].kind == std::string("block"))) {
                                        out = (out + (*this).render_stmt(stmt.children[INT64_C(1)], indent_level));
                                    }
                                    else {
                                        out = (out + std::string("{\n"));
                                        out = (out + (*this).render_stmt(stmt.children[INT64_C(1)], (indent_level + INT64_C(1))));
                                        out = (out + ((std::string("") + (indent)) + std::string("}\n")));
                                    }
                                }
                                return out;
                            }
                            else {
                                if ((stmt.kind == std::string("while"))) {
                                    std::string out = ((((std::string("") + (indent)) + std::string("while (")) + ((*this).render_expr(stmt.exprs[INT64_C(0)]))) + std::string(") "));
                                    if ((static_cast<int64_t>(stmt.children.size()) > INT64_C(0)) && (stmt.children[INT64_C(0)].kind == std::string("block"))) {
                                        out = (out + (*this).render_stmt(stmt.children[INT64_C(0)], indent_level));
                                    }
                                    else {
                                        out = (out + std::string("{\n"));
                                        if ((static_cast<int64_t>(stmt.children.size()) > INT64_C(0))) {
                                            out = (out + (*this).render_stmt(stmt.children[INT64_C(0)], (indent_level + INT64_C(1))));
                                        }
                                        out = (out + ((std::string("") + (indent)) + std::string("}\n")));
                                    }
                                    return out;
                                }
                                else {
                                    if ((stmt.kind == std::string("break"))) {
                                        return ((std::string("") + (indent)) + std::string("break;\n"));
                                    }
                                    else {
                                        if ((stmt.kind == std::string("continue"))) {
                                            return ((std::string("") + (indent)) + std::string("continue;\n"));
                                        }
                                        else {
                                            if ((stmt.kind == std::string("pass"))) {
                                                return std::string("");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return ((std::string("") + (indent)) + std::string("/* unsupported lambda stmt */\n"));
    }

};

std::string cpp_render_typed_expr_inline(const TypedExpr& expr, const std::vector<TypedLambdaBodyInfo>& lambda_blocks) {
    auto renderer = TypedLambdaCppRenderer(lambda_blocks);
    return renderer.render_expr(expr);
}

CppStruct cpp_lower_struct(const TypedStructInfo& struct_info) {
    std::vector<CppField> fields = {};
    std::vector<CppParam> constructor_params = {};
    std::vector<std::string> constructor_defaults = {};
    std::vector<std::string> init_fields = {};
    std::vector<std::string> constructor_body_lines = {};
    for (const auto& field : struct_info.fields) {
        fields.push_back(CppField(field.name, cpp_type_from_type_node(field.field_type)));
    }
    for (const auto& param : struct_info.constructor_params) {
        constructor_params.push_back(CppParam(param.name.lexeme, cpp_type_from_type_node(param.param_type)));
    }
    for (const auto& default_expr : struct_info.constructor_defaults) {
        {
            const auto& _match_131 = default_expr;
            if (_match_131._tag == "None") {
                constructor_defaults.push_back(std::string(""));
            }
            else {
                constructor_defaults.push_back(cpp_expr_literal(default_expr));
            }
        }
    }
    for (const auto& stmt : struct_info.constructor_body) {
        if ((stmt.kind == std::string("expr_stmt")) && (static_cast<int64_t>(stmt.exprs.size()) > INT64_C(0))) {
            const auto& expr = stmt.exprs[INT64_C(0)];
            if ((expr.kind == std::string("set")) && (static_cast<int64_t>(expr.children.size()) >= INT64_C(2))) {
                const auto& object = expr.children[INT64_C(0)];
                const auto& value = expr.children[INT64_C(1)];
                if ((object.kind == std::string("this"))) {
                    if ((value.kind == std::string("variable"))) {
                        init_fields.push_back(((((std::string("") + (expr.text_a)) + std::string("(")) + (value.text_a)) + std::string(")")));
                    }
                    else {
                        constructor_body_lines.push_back(((((std::string("        ") + (expr.text_a)) + std::string(" = ")) + (cpp_typed_expr_literal(value))) + std::string(";")));
                    }
                }
            }
        }
    }
    return CppStruct(struct_info.name, fields, constructor_params, constructor_defaults, init_fields, constructor_body_lines);
}

CppEnum cpp_lower_enum(const TypedEnumInfo& enum_info, bool has_to_string) {
    std::vector<CppEnumVariant> variants = {};
    for (const auto& variant : enum_info.variants) {
        std::vector<CppField> fields = {};
        for (int64_t i = INT64_C(0); i < static_cast<int64_t>(variant.types.size()); i++) {
            fields.push_back(CppField(variant.field_names[i], cpp_type_from_type_node(variant.types[i])));
        }
        variants.push_back(CppEnumVariant(variant.name, fields));
    }
    return CppEnum(enum_info.name, variants, has_to_string);
}

CppModule cpp_lower_module(const TypedProgram& program, const IrModule& module_ir) {
    std::vector<std::string> includes = std::vector{std::string("#include \"lavina.h\"")};
    std::vector<CppStruct> structs = {};
    std::vector<CppEnum> enums = {};
    std::vector<CppFunction> functions = {};
    for (const auto& struct_info : program.structs) {
        structs.push_back(cpp_lower_struct(struct_info));
    }
    for (const auto& enum_info : program.enums) {
        bool has_to_string = false;
        for (const auto& fn_ir : ir_module_functions(module_ir)) {
            if ((ir_function_name(fn_ir) == ((std::string("") + (enum_info.name)) + std::string("__to_string")))) {
                has_to_string = true;
            }
        }
        enums.push_back(cpp_lower_enum(enum_info, has_to_string));
    }
    auto module_functions = ir_module_functions(module_ir);
    auto module_closures = ir_module_closures(module_ir);
    for (const auto& fn_ir : module_functions) {
        if ((ir_function_name(fn_ir) == std::string("__module_init"))) {
            continue;
        }
        functions.push_back(cpp_lower_function(fn_ir, module_closures));
    }
    return CppModule(includes, structs, enums, functions);
}

std::string cpp_render_struct(const CppStruct& struct_model) {
    std::vector<std::string> lines = {};
    lines.push_back(((std::string("struct ") + (struct_model.name)) + std::string(" {")));
    for (const auto& field : struct_model.fields) {
        lines.push_back(((((std::string("    ") + (field.field_type)) + std::string(" ")) + (field.name)) + std::string(";")));
    }
    lines.push_back(std::string(""));
    lines.push_back(((std::string("    ") + (struct_model.name)) + std::string("() = default;")));
    if ((static_cast<int64_t>(struct_model.constructor_params.size()) > INT64_C(0))) {
        std::vector<std::string> params = {};
        for (int64_t i = INT64_C(0); i < static_cast<int64_t>(struct_model.constructor_params.size()); i++) {
            std::string param = ((((std::string("") + (struct_model.constructor_params[i].param_type)) + std::string(" ")) + (struct_model.constructor_params[i].name)) + std::string(""));
            if ((i < static_cast<int64_t>(struct_model.constructor_defaults.size())) && (struct_model.constructor_defaults[i] != std::string(""))) {
                param = (param + ((std::string(" = ") + (struct_model.constructor_defaults[i])) + std::string("")));
            }
            params.push_back(param);
        }
        if ((static_cast<int64_t>(struct_model.init_fields.size()) > INT64_C(0))) {
            lines.push_back(std::string(""));
            lines.push_back(((((((std::string("    ") + (struct_model.name)) + std::string("(")) + (lv_join(params, std::string(", ")))) + std::string(") : ")) + (lv_join(struct_model.init_fields, std::string(", ")))) + std::string(" {")));
            for (const auto& line : struct_model.constructor_body_lines) {
                lines.push_back(line);
            }
            lines.push_back(std::string("    }"));
        }
        else {
            lines.push_back(std::string(""));
            lines.push_back(((((std::string("    ") + (struct_model.name)) + std::string("(")) + (lv_join(params, std::string(", ")))) + std::string(") {")));
            for (const auto& line : struct_model.constructor_body_lines) {
                lines.push_back(line);
            }
            lines.push_back(std::string("    }"));
        }
    }
    lines.push_back(std::string("};"));
    return lv_join(lines, std::string("\n"));
}

std::string cpp_render_enum(const CppEnum& enum_model) {
    std::vector<std::string> lines = {};
    lines.push_back(((std::string("struct ") + (enum_model.name)) + std::string(" {")));
    for (const auto& variant : enum_model.variants) {
        if ((static_cast<int64_t>(variant.fields.size()) == INT64_C(0))) {
            lines.push_back(((std::string("    struct ") + (variant.name)) + std::string(" {};")));
        }
        else {
            std::vector<std::string> field_parts = {};
            for (const auto& field : variant.fields) {
                field_parts.push_back(((((std::string("") + (field.field_type)) + std::string(" ")) + (field.name)) + std::string(";")));
            }
            lines.push_back(((((std::string("    struct ") + (variant.name)) + std::string(" { ")) + (lv_join(field_parts, std::string(" ")))) + std::string(" };")));
        }
    }
    lines.push_back(std::string(""));
    lines.push_back(std::string("    std::string _tag;"));
    std::vector<std::string> variant_types = {};
    for (const auto& variant : enum_model.variants) {
        variant_types.push_back(((((std::string("") + (enum_model.name)) + std::string("::")) + (variant.name)) + std::string("")));
    }
    lines.push_back(((std::string("    std::variant<") + (lv_join(variant_types, std::string(", ")))) + std::string("> _data;")));
    lines.push_back(std::string(""));
    for (const auto& variant : enum_model.variants) {
        if ((static_cast<int64_t>(variant.fields.size()) == INT64_C(0))) {
            lines.push_back(((((((((std::string("    static ") + (enum_model.name)) + std::string(" make_")) + (variant.name)) + std::string("() { return {\"")) + (variant.name)) + std::string("\", ")) + (variant.name)) + std::string("{}}; }")));
        }
        else {
            std::vector<std::string> params = {};
            std::vector<std::string> args = {};
            for (const auto& field : variant.fields) {
                params.push_back(((((std::string("") + (field.field_type)) + std::string(" ")) + (field.name)) + std::string("")));
                args.push_back(field.name);
            }
            lines.push_back(((((((((((((std::string("    static ") + (enum_model.name)) + std::string(" make_")) + (variant.name)) + std::string("(")) + (lv_join(params, std::string(", ")))) + std::string(") { return {\"")) + (variant.name)) + std::string("\", ")) + (variant.name)) + std::string("{")) + (lv_join(args, std::string(", ")))) + std::string("}}; }")));
        }
    }
    lines.push_back(std::string(""));
    lines.push_back(std::string("    std::string operator[](const std::string& key) const {"));
    lines.push_back(std::string("        if (key == \"_tag\") return _tag;"));
    lines.push_back(std::string("        return \"\";"));
    lines.push_back(std::string("    }"));
    lines.push_back(std::string("};"));
    if (enum_model.has_to_string) {
        lines.push_back(((((std::string("std::string ") + (enum_model.name)) + std::string("__to_string(")) + (enum_model.name)) + std::string("& self);")));
        lines.push_back(((((((std::string("inline void print(const ") + (enum_model.name)) + std::string("& _v) { std::cout << ")) + (enum_model.name)) + std::string("__to_string(const_cast<")) + (enum_model.name)) + std::string("&>(_v)) << std::endl; }")));
        lines.push_back(((((((std::string("inline std::string operator+(const std::string& _s, const ") + (enum_model.name)) + std::string("& _v) { return _s + ")) + (enum_model.name)) + std::string("__to_string(const_cast<")) + (enum_model.name)) + std::string("&>(_v)); }")));
        lines.push_back(((((((std::string("inline std::string operator+(const ") + (enum_model.name)) + std::string("& _v, const std::string& _s) { return ")) + (enum_model.name)) + std::string("__to_string(const_cast<")) + (enum_model.name)) + std::string("&>(_v)) + _s; }")));
    }
    else {
        lines.push_back(((((std::string("inline void print(const ") + (enum_model.name)) + std::string("& _v) { std::cout << \"")) + (enum_model.name)) + std::string("(\" << _v._tag << \")\" << std::endl; }")));
        lines.push_back(((std::string("inline std::string operator+(const std::string& _s, const ") + (enum_model.name)) + std::string("& _v) { return _s + _v._tag; }")));
        lines.push_back(((std::string("inline std::string operator+(const ") + (enum_model.name)) + std::string("& _v, const std::string& _s) { return _v._tag + _s; }")));
    }
    return lv_join(lines, std::string("\n"));
}

std::string cpp_render_function(const CppFunction& fn_model) {
    std::vector<std::string> lines = {};
    std::vector<std::string> params = {};
    for (const auto& param : fn_model.params) {
        params.push_back(((((std::string("") + (param.param_type)) + std::string(" ")) + (param.name)) + std::string("")));
    }
    lines.push_back(((((((std::string("") + (fn_model.return_type)) + std::string(" ")) + (fn_model.name)) + std::string("(")) + (lv_join(params, std::string(", ")))) + std::string(") {")));
    for (const auto& local : fn_model.locals) {
        lines.push_back(((((std::string("    ") + (local.local_type)) + std::string(" ")) + (local.name)) + std::string(";")));
    }
    if ((static_cast<int64_t>(fn_model.locals.size()) > INT64_C(0)) && (static_cast<int64_t>(fn_model.body_lines.size()) > INT64_C(0))) {
        lines.push_back(std::string(""));
    }
    for (const auto& line : fn_model.body_lines) {
        lines.push_back(line);
    }
    lines.push_back(std::string("}"));
    return lv_join(lines, std::string("\n"));
}

std::string cpp_render_function_decl(const CppFunction& fn_model) {
    if ((fn_model.return_type == std::string("auto"))) {
        return std::string("");
    }
    std::vector<std::string> params = {};
    for (const auto& param : fn_model.params) {
        params.push_back(((((std::string("") + (param.param_type)) + std::string(" ")) + (param.name)) + std::string("")));
    }
    return ((((((std::string("") + (fn_model.return_type)) + std::string(" ")) + (fn_model.name)) + std::string("(")) + (lv_join(params, std::string(", ")))) + std::string(");"));
}

std::string cpp_render_module(const CppModule& module_model) {
    std::vector<std::string> lines = {};
    for (const auto& inc : module_model.includes) {
        lines.push_back(inc);
    }
    lines.push_back(std::string(""));
    for (int64_t i = INT64_C(0); i < static_cast<int64_t>(module_model.structs.size()); i++) {
        lines.push_back(cpp_render_struct(module_model.structs[i]));
        lines.push_back(std::string(""));
    }
    for (int64_t i = INT64_C(0); i < static_cast<int64_t>(module_model.enums.size()); i++) {
        lines.push_back(cpp_render_enum(module_model.enums[i]));
        lines.push_back(std::string(""));
    }
    for (const auto& function_model : module_model.functions) {
        std::string decl = cpp_render_function_decl(function_model);
        if ((decl != std::string(""))) {
            lines.push_back(decl);
        }
    }
    if ((static_cast<int64_t>(module_model.functions.size()) > INT64_C(0))) {
        lines.push_back(std::string(""));
    }
    for (int64_t i = INT64_C(0); i < static_cast<int64_t>(module_model.functions.size()); i++) {
        lines.push_back(cpp_render_function(module_model.functions[i]));
        if (((i + INT64_C(1)) < static_cast<int64_t>(module_model.functions.size()))) {
            lines.push_back(std::string(""));
        }
    }
    return lv_join(lines, std::string("\n"));
}

std::string lower_ir_to_cpp(const TypedProgram& program, const IrModule& module_ir) {
    auto module_model = cpp_lower_module(program, module_ir);
    return cpp_render_module(module_model);
}

std::vector<std::string> ir_named_param_parts(const std::vector<IrParam>& params) {
    std::vector<std::string> parts = {};
    for (const auto& p : params) {
        parts.push_back(((((std::string("") + (p.name)) + std::string(": ")) + (ir_type_to_string(p.param_type))) + std::string("")));
    }
    return parts;
}

std::vector<std::string> ir_named_signature_param_parts(const std::vector<IrParam>& params, const std::vector<IrType>& param_types) {
    std::vector<std::string> parts = {};
    for (int64_t i = INT64_C(0); i < static_cast<int64_t>(params.size()); i++) {
        if ((i < static_cast<int64_t>(param_types.size()))) {
            parts.push_back(((((std::string("") + (params[i].name)) + std::string(": ")) + (ir_type_to_string(param_types[i]))) + std::string("")));
        }
    }
    return parts;
}

std::string ir_call_target_to_string(const IrCallTarget& target) {
    if (ir_call_target_is_intrinsic(target)) {
        return ((std::string("intrinsic ") + (ir_call_target_name(target))) + std::string(""));
    }
    else {
        if (ir_call_target_is_closure(target)) {
            return ((std::string("closure ") + (ir_call_target_name(target))) + std::string(""));
        }
        else {
            if (ir_call_target_is_indirect(target)) {
                return ((std::string("indirect ") + (ir_call_target_name(target))) + std::string(""));
            }
        }
    }
    return ir_call_target_name(target);
}

std::string ir_inst_to_string(const IrInst& inst) {
    if (ir_inst_is_const(inst)) {
        return ((((((((std::string("") + (ir_inst_dest(inst))) + std::string(" = const ")) + (ir_type_to_string(ir_inst_value_type(inst)))) + std::string(" ")) + (ir_inst_literal_kind(inst))) + std::string(" ")) + (ir_inst_literal_value(inst))) + std::string(""));
    }
    else {
        if (ir_inst_is_vector_literal(inst)) {
            return ((((((std::string("") + (ir_inst_dest(inst))) + std::string(" = vector_literal [")) + (lv_join(ir_inst_args(inst), std::string(", ")))) + std::string("] : ")) + (ir_type_to_string(ir_inst_value_type(inst)))) + std::string(""));
        }
        else {
            if (ir_inst_is_map_literal(inst)) {
                return ((((((std::string("") + (ir_inst_dest(inst))) + std::string(" = map_literal {")) + (lv_join(ir_inst_args(inst), std::string(", ")))) + std::string("} : ")) + (ir_type_to_string(ir_inst_value_type(inst)))) + std::string(""));
            }
            else {
                if (ir_inst_is_move(inst)) {
                    return ((((((std::string("") + (ir_inst_dest(inst))) + std::string(" = move ")) + (ir_inst_source_name(inst))) + std::string(" : ")) + (ir_type_to_string(ir_inst_value_type(inst)))) + std::string(""));
                }
                else {
                    if (ir_inst_is_unary(inst)) {
                        return ((((((((std::string("") + (ir_inst_dest(inst))) + std::string(" = unary ")) + (ir_inst_unary_op(inst))) + std::string(" ")) + (ir_inst_unary_operand(inst))) + std::string(" : ")) + (ir_type_to_string(ir_inst_value_type(inst)))) + std::string(""));
                    }
                    else {
                        if (ir_inst_is_binary(inst)) {
                            return ((((((((((std::string("") + (ir_inst_dest(inst))) + std::string(" = binary ")) + (ir_inst_binary_op(inst))) + std::string(" ")) + (ir_inst_binary_left(inst))) + std::string(", ")) + (ir_inst_binary_right(inst))) + std::string(" : ")) + (ir_type_to_string(ir_inst_value_type(inst)))) + std::string(""));
                        }
                        else {
                            if (ir_inst_is_call_like(inst)) {
                                std::string rendered_target = ir_call_target_to_string(ir_inst_call_target(inst));
                                std::string rendered_args = lv_join(ir_inst_args(inst), std::string(", "));
                                std::string call_mode = ir_inst_call_mode(inst);
                                if ((call_mode == std::string("value"))) {
                                    return ((((((((std::string("") + (ir_inst_dest(inst))) + std::string(" = call ")) + (rendered_target)) + std::string("(")) + (rendered_args)) + std::string(") : ")) + (ir_type_to_string(ir_inst_value_type(inst)))) + std::string(""));
                                }
                                else {
                                    if ((call_mode == std::string("auto"))) {
                                        return ((((((std::string("") + (ir_inst_dest(inst))) + std::string(" = call_auto ")) + (rendered_target)) + std::string("(")) + (rendered_args)) + std::string(")"));
                                    }
                                }
                                return ((((std::string("call ") + (rendered_target)) + std::string("(")) + (rendered_args)) + std::string(") : void"));
                            }
                            else {
                                if (ir_inst_is_field_read(inst)) {
                                    return ((((((((std::string("") + (ir_inst_dest(inst))) + std::string(" = get_field ")) + (ir_inst_field_object(inst))) + std::string(".")) + (ir_inst_field_name(inst))) + std::string(" : ")) + (ir_type_to_string(ir_inst_value_type(inst)))) + std::string(""));
                                }
                                else {
                                    if (ir_inst_is_field_write(inst)) {
                                        return ((((((std::string("set_field ") + (ir_inst_field_object(inst))) + std::string(".")) + (ir_inst_field_name(inst))) + std::string(" = ")) + (ir_inst_field_value(inst))) + std::string(""));
                                    }
                                    else {
                                        if (ir_inst_is_index_read(inst)) {
                                            return ((((((((std::string("") + (ir_inst_dest(inst))) + std::string(" = index ")) + (ir_inst_index_object(inst))) + std::string("[")) + (ir_inst_index_value(inst))) + std::string("] : ")) + (ir_type_to_string(ir_inst_value_type(inst)))) + std::string(""));
                                        }
                                        else {
                                            if (ir_inst_is_index_write(inst)) {
                                                return ((((((std::string("index_set ") + (ir_inst_index_object(inst))) + std::string("[")) + (ir_inst_index_value(inst))) + std::string("] = ")) + (ir_inst_index_set_value(inst))) + std::string(""));
                                            }
                                            else {
                                                if (ir_inst_is_convert(inst)) {
                                                    return ((((((std::string("") + (ir_inst_dest(inst))) + std::string(" = convert ")) + (ir_inst_source_name(inst))) + std::string(" : ")) + (ir_type_to_string(ir_inst_value_type(inst)))) + std::string(""));
                                                }
                                                else {
                                                    if (ir_inst_is_bind_auto(inst)) {
                                                        return ((((std::string("") + (ir_inst_dest(inst))) + std::string(" = bind_auto ")) + (ir_inst_bind_expr(inst))) + std::string(""));
                                                    }
                                                    else {
                                                        if (ir_inst_is_make_closure(inst)) {
                                                            return ((((((((std::string("") + (ir_inst_dest(inst))) + std::string(" = make_closure ")) + (ir_inst_closure_name(inst))) + std::string("(")) + (lv_join(ir_inst_args(inst), std::string(", ")))) + std::string(") : ")) + (ir_type_to_string(ir_inst_value_type(inst)))) + std::string(""));
                                                        }
                                                        else {
                                                            if (ir_inst_is_enum_check(inst)) {
                                                                return ((((((std::string("") + (ir_inst_dest(inst))) + std::string(" = enum_is ")) + (ir_inst_enum_object(inst))) + std::string(" is ")) + (ir_inst_enum_variant(inst))) + std::string(""));
                                                            }
                                                            else {
                                                                if (ir_inst_is_enum_read(inst)) {
                                                                    return ((((((((((std::string("") + (ir_inst_dest(inst))) + std::string(" = enum_get ")) + (ir_inst_enum_object(inst))) + std::string(" as ")) + (ir_inst_enum_variant(inst))) + std::string(".")) + (ir_inst_enum_field(inst))) + std::string(" : ")) + (ir_type_to_string(ir_inst_value_type(inst)))) + std::string(""));
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return std::string("<unknown-inst>");
}

std::string ir_terminator_to_string(const IrTerminator& term) {
    if (ir_term_is_none(term)) {
        return std::string("unreachable");
    }
    else {
        if (ir_term_is_jump(term)) {
            return ((std::string("jump ") + (ir_term_jump_target(term))) + std::string(""));
        }
        else {
            if (ir_term_is_branch(term)) {
                return ((((((std::string("branch ") + (ir_term_condition(term))) + std::string(", ")) + (ir_term_then_target(term))) + std::string(", ")) + (ir_term_else_target(term))) + std::string(""));
            }
            else {
                if (ir_term_is_return_void(term)) {
                    return std::string("return");
                }
                else {
                    if (ir_term_is_return_value(term)) {
                        return ((std::string("return ") + (ir_term_return_value_name(term))) + std::string(""));
                    }
                    else {
                        if (ir_term_is_unreachable(term)) {
                            return std::string("unreachable");
                        }
                    }
                }
            }
        }
    }
    return std::string("<unknown-term>");
}

std::string ir_dump_function(const IrFunction& fn_ir) {
    std::vector<std::string> parts = {};
    std::vector<std::string> param_parts = ir_named_param_parts(ir_function_params(fn_ir));
    parts.push_back(((((((std::string("fn ") + (ir_function_name(fn_ir))) + std::string("(")) + (lv_join(param_parts, std::string(", ")))) + std::string(") -> ")) + (ir_type_to_string(ir_function_return_type(fn_ir)))) + std::string(" {")));
    auto locals = ir_function_locals(fn_ir);
    if ((static_cast<int64_t>(locals.size()) > INT64_C(0))) {
        std::vector<std::string> local_parts = {};
        for (const auto& local : locals) {
            local_parts.push_back(((((std::string("") + (local.name)) + std::string(": ")) + (ir_type_to_string(local.local_type))) + std::string("")));
        }
        parts.push_back(((std::string("  locals ") + (lv_join(local_parts, std::string(", ")))) + std::string("")));
    }
    for (const auto& block : ir_function_blocks(fn_ir)) {
        parts.push_back(((std::string("  ") + (ir_block_name(block))) + std::string(":")));
        for (const auto& inst : ir_block_instructions(block)) {
            parts.push_back(((std::string("    ") + (ir_inst_to_string(inst))) + std::string("")));
        }
        parts.push_back(((std::string("    ") + (ir_terminator_to_string(ir_block_terminator(block)))) + std::string("")));
    }
    parts.push_back(std::string("}"));
    return lv_join(parts, std::string("\n"));
}

std::string ir_dump_closure(const IrClosureDef& closure_ir) {
    auto signature = ir_closure_signature(closure_ir);
    std::vector<std::string> capture_parts = ir_named_param_parts(ir_closure_capture_params(closure_ir));
    std::vector<std::string> lambda_parts = ir_named_signature_param_parts(ir_closure_lambda_params(closure_ir), ir_closure_sig_param_types(signature));
    return ((((((((((std::string("closure ") + (ir_closure_name(closure_ir))) + std::string(" captures (")) + (lv_join(capture_parts, std::string(", ")))) + std::string(") params (")) + (lv_join(lambda_parts, std::string(", ")))) + std::string(") -> ")) + (ir_type_to_string(ir_closure_sig_return_type(signature)))) + std::string(" uses ")) + (ir_closure_helper_name(closure_ir))) + std::string(""));
}

std::string ir_dump_module(const IrModule& module_ir) {
    std::vector<std::string> parts = {};
    parts.push_back(((std::string("module ") + (ir_module_name(module_ir))) + std::string("")));
    parts.push_back(std::string(""));
    auto closures = ir_module_closures(module_ir);
    for (const auto& closure_ir : closures) {
        parts.push_back(ir_dump_closure(closure_ir));
    }
    if ((static_cast<int64_t>(closures.size()) > INT64_C(0))) {
        parts.push_back(std::string(""));
    }
    auto functions = ir_module_functions(module_ir);
    for (int64_t i = INT64_C(0); i < static_cast<int64_t>(functions.size()); i++) {
        parts.push_back(ir_dump_function(functions[i]));
        if (((i + INT64_C(1)) < static_cast<int64_t>(functions.size()))) {
            parts.push_back(std::string(""));
        }
    }
    return lv_join(parts, std::string("\n"));
}

IrCallSig ir_call_sig_from_lowered_types(const std::vector<IrType>& param_types, IrType return_type) {
    return ir_call_sig(param_types, return_type);
}

IrClosureSig ir_closure_sig_from_lowered_types(const std::vector<IrType>& param_types, IrType return_type) {
    return ir_closure_sig(param_types, return_type);
}

IrCallTarget ir_call_target_from_typed(const TypedCallTarget& target, IrCallSig signature) {
    {
        const auto& _match_132 = target.kind;
        if (std::holds_alternative<std::decay_t<decltype(_match_132)>::Intrinsic>(_match_132._data)) {
            return ir_call_target_intrinsic(target.lowered_name, signature);
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_132)>::DirectFunction>(_match_132._data)) {
            return ir_call_target_function(target.lowered_name, signature);
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_132)>::DirectConstructor>(_match_132._data)) {
            return ir_call_target_function(target.lowered_name, signature);
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_132)>::ClosureValue>(_match_132._data)) {
            return ir_call_target_closure(target.lowered_name, signature);
        }
        else {
            return ir_call_target_indirect(target.lowered_name, signature);
        }
    }
}

IrCallTarget ir_len_call_target(IrType collection_type) {
    std::vector<IrType> param_types = std::vector{collection_type};
    return ir_call_target_intrinsic(std::string("len"), ir_call_sig(param_types, ir_int_type()));
}

void ir_emit_call_auto_named(std::vector<IrInst>& instructions, std::string dest_name, const TypedExpr& expr, const std::vector<std::string>& lowered_args, IrCallTarget call_target) {
    instructions.push_back(ir_call_auto(dest_name, call_target, lowered_args));
}

void ir_emit_call_void_expr(std::vector<IrInst>& instructions, const TypedExpr& expr, const std::vector<std::string>& lowered_args, IrCallTarget call_target) {
    instructions.push_back(ir_call_void(call_target, lowered_args));
}

void ir_emit_return_from_exprs(IrTerminator& terminator, const std::vector<TypedExpr>& exprs, std::string value_name) {
    if ((static_cast<int64_t>(exprs.size()) == INT64_C(0))) {
        terminator = ir_term_return_void();
        return;
    }
    if ((exprs[INT64_C(0)].kind == std::string("none"))) {
        terminator = ir_term_return_void();
        return;
    }
    terminator = ir_term_return_value(value_name);
}

std::vector<IrParam> ir_params_from_names_and_types(const std::vector<std::string>& names, const std::vector<IrType>& types) {
    std::vector<IrParam> params = {};
    for (int64_t i = INT64_C(0); i < static_cast<int64_t>(names.size()); i++) {
        if ((i < static_cast<int64_t>(types.size()))) {
            params.push_back(IrParam(names[i], types[i]));
        }
    }
    return params;
}

std::vector<std::string> ir_capture_names(const TypedExpr& expr) {
    std::vector<std::string> captures = {};
    for (const auto& capture_name : expr.capture_names) {
        captures.push_back(capture_name);
    }
    return captures;
}

IrType ir_resolve_call_result_type(IrType result_type, std::unordered_map<std::string, IrType>& call_return_types, std::string lowered_name, IrType current_return_type) {
    if (ir_type_is_auto(result_type) && (call_return_types.count(lowered_name) > 0)) {
        result_type = call_return_types[lowered_name];
    }
    if (ir_type_is_auto(result_type) && (!ir_type_is_auto(current_return_type)) && (!ir_type_is_void(current_return_type))) {
        result_type = current_return_type;
    }
    return result_type;
}

struct IrIfPlan {
    int64_t then_index;
    int64_t else_index;
    int64_t end_index;
    std::string then_name;
    std::string else_name;
    std::string end_name;

};

struct IrWhilePlan {
    int64_t cond_index;
    int64_t body_index;
    int64_t end_index;
    std::string cond_name;
    std::string body_name;
    std::string end_name;

};

struct IrForPlan {
    int64_t cond_index;
    int64_t body_index;
    int64_t step_index;
    int64_t end_index;
    std::string cond_name;
    std::string body_name;
    std::string step_name;
    std::string end_name;

};

struct IrMatchArmPlan {
    int64_t body_index;
    int64_t fail_index;
    std::string body_name;
    std::string fail_name;
    bool is_wildcard;

};

IrIfPlan ir_if_plan(const std::vector<IrBlock>& blocks, int64_t then_index, int64_t else_index, int64_t end_index) {
    return IrIfPlan(then_index, else_index, end_index, blocks[then_index].name, blocks[else_index].name, blocks[end_index].name);
}

IrWhilePlan ir_while_plan(const std::vector<IrBlock>& blocks, int64_t cond_index, int64_t body_index, int64_t end_index) {
    return IrWhilePlan(cond_index, body_index, end_index, blocks[cond_index].name, blocks[body_index].name, blocks[end_index].name);
}

IrForPlan ir_for_plan(const std::vector<IrBlock>& blocks, int64_t cond_index, int64_t body_index, int64_t step_index, int64_t end_index) {
    return IrForPlan(cond_index, body_index, step_index, end_index, blocks[cond_index].name, blocks[body_index].name, blocks[step_index].name, blocks[end_index].name);
}

IrMatchArmPlan ir_match_arm_plan(const std::vector<IrBlock>& blocks, int64_t body_index, int64_t fail_index, bool is_wildcard) {
    return IrMatchArmPlan(body_index, fail_index, blocks[body_index].name, blocks[fail_index].name, is_wildcard);
}

void ir_push_loop_targets(std::vector<std::string>& break_targets, std::vector<std::string>& continue_targets, std::string break_target, std::string continue_target) {
    break_targets.push_back(break_target);
    continue_targets.push_back(continue_target);
}

void ir_pop_loop_targets(std::vector<std::string>& break_targets, std::vector<std::string>& continue_targets) {
    if ((static_cast<int64_t>(break_targets.size()) > INT64_C(0))) {
        lv_pop(break_targets);
    }
    if ((static_cast<int64_t>(continue_targets.size()) > INT64_C(0))) {
        lv_pop(continue_targets);
    }
}

std::string ir_current_break_target(const std::vector<std::string>& break_targets) {
    if ((static_cast<int64_t>(break_targets.size()) == INT64_C(0))) {
        return std::string("");
    }
    return break_targets[(static_cast<int64_t>(break_targets.size()) - INT64_C(1))];
}

std::string ir_current_continue_target(const std::vector<std::string>& continue_targets) {
    if ((static_cast<int64_t>(continue_targets.size()) == INT64_C(0))) {
        return std::string("");
    }
    return continue_targets[(static_cast<int64_t>(continue_targets.size()) - INT64_C(1))];
}

IrType ir_element_type_of(const IrType& container_type) {
    return intrinsic_ir_element_type(container_type);
}

IrType ir_struct_field_result_type(IrType fallback_type, std::unordered_map<std::string, IrType>& struct_field_types, const IrType& object_type, std::string field_name) {
    if ((fallback_type.name != std::string("auto"))) {
        return fallback_type;
    }
    std::string field_key = ((((std::string("") + (object_type.name)) + std::string(".")) + (field_name)) + std::string(""));
    if ((struct_field_types.count(field_key) > 0)) {
        return struct_field_types[field_key];
    }
    return fallback_type;
}

TypedEnumVariant ir_enum_variant_lookup(std::unordered_map<std::string, std::vector<TypedEnumVariant>>& enum_variants, std::string enum_name, std::string variant_name) {
    std::vector<TypeNode> empty_types = {};
    std::vector<std::string> empty_fields = {};
    if ((enum_variants.count(enum_name) > 0)) {
        for (const auto& variant : enum_variants[enum_name]) {
            if ((variant.name == variant_name)) {
                return variant;
            }
        }
    }
    return TypedEnumVariant(std::string(""), empty_types, empty_fields);
}

std::string ir_match_enum_name(const TypeNode& value_type) {
    {
        const auto& _match_133 = value_type;
        if (std::holds_alternative<std::decay_t<decltype(_match_133)>::Custom>(_match_133._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_133)>::Custom>(_match_133._data);
            auto& name = _v.name;
            auto& type_args = _v.type_args;
            return name;
        }
        else {
            return std::string("");
        }
    }
}

void ir_emit_named_vector_literal(std::vector<IrInst>& instructions, std::string dest_name, IrType declared_type, const std::vector<std::string>& lowered_elements) {
    instructions.push_back(ir_vector_literal(dest_name, declared_type, lowered_elements));
}

void ir_emit_named_map_literal(std::vector<IrInst>& instructions, std::string dest_name, IrType declared_type, const std::vector<std::string>& lowered_items) {
    instructions.push_back(ir_map_literal(dest_name, declared_type, lowered_items));
}

struct LoweredValue {
    std::string name;
    IrType value_type;

};

struct IrLowerResult {
    IrModule module;
    std::vector<std::string> errors;

};

struct IrLowerer {
    int64_t temp_counter;
    int64_t block_counter;
    int64_t closure_counter;
    std::vector<std::string> errors;
    std::vector<IrBlock> active_blocks;
    int64_t current_block;
    std::vector<IrLocal> active_locals;
    std::unordered_map<std::string, IrType> local_types;
    std::unordered_map<std::string, IrType> struct_field_types;
    std::unordered_map<std::string, IrType> call_return_types;
    std::unordered_map<std::string, std::vector<TypedEnumVariant>> enum_variants;
    std::vector<TypedLambdaBodyInfo> lambda_blocks;
    std::vector<IrClosureDef> closure_defs;
    std::vector<IrFunction> closure_functions;
    IrType current_return_type;
    std::vector<std::string> break_targets;
    std::vector<std::string> continue_targets;

    IrLowerer() {
        this->temp_counter = INT64_C(0);
        this->block_counter = INT64_C(0);
        this->closure_counter = INT64_C(0);
        this->errors = {};
        this->active_blocks = {};
        this->current_block = INT64_C(0);
        this->active_locals = {};
        this->local_types = {{}};
        this->struct_field_types = {{}};
        this->call_return_types = {{}};
        this->enum_variants = {{}};
        this->lambda_blocks = {};
        this->closure_defs = {};
        this->closure_functions = {};
        this->current_return_type = ir_void_type();
        this->break_targets = {};
        this->continue_targets = {};
    }

    std::string next_temp() {
        std::string name = ((std::string("%") + (this->temp_counter)) + std::string(""));
        this->temp_counter = (this->temp_counter + INT64_C(1));
        return name;
    }

    std::string new_temp(IrType value_type) {
        std::string name = (*this).next_temp();
        (*this).declare_local(name, value_type);
        return name;
    }

    std::string next_block_name(std::string prefix) {
        std::string name = ((((std::string("") + (prefix)) + std::string("")) + (this->block_counter)) + std::string(""));
        this->block_counter = (this->block_counter + INT64_C(1));
        return name;
    }

    int64_t create_block(std::string prefix) {
        std::string name = (*this).next_block_name(prefix);
        std::vector<IrInst> instructions = {};
        this->active_blocks.push_back(IrBlock(name, instructions, ir_term_none()));
        return (static_cast<int64_t>(this->active_blocks.size()) - INT64_C(1));
    }

    bool block_terminated(int64_t index) {
        return (this->active_blocks[index].terminator.kind != std::string("none"));
    }

    void emit(IrInst inst) {
        auto& block = this->active_blocks[this->current_block];
        block.instructions.push_back(inst);
    }

    void terminate(IrTerminator term) {
        auto& block = this->active_blocks[this->current_block];
        block.terminator = term;
    }

    void switch_to(int64_t index) {
        this->current_block = index;
    }

    void declare_local(std::string name, IrType local_type) {
        if ((this->local_types.count(name) > 0)) {
            return;
        }
        this->local_types[name] = local_type;
        this->active_locals.push_back(IrLocal(name, local_type));
    }

    IrType lower_type(const TypeNode& t) {
        {
            const auto& _match_134 = t;
            if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Void>(_match_134._data)) {
                return ir_void_type();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Bool>(_match_134._data)) {
                return ir_bool_type();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Int>(_match_134._data)) {
                return ir_int_type();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Int8>(_match_134._data)) {
                return ir_int8_type();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Int16>(_match_134._data)) {
                return ir_int16_type();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Int32>(_match_134._data)) {
                return ir_int32_type();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::USize>(_match_134._data)) {
                return ir_usize_type();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Float>(_match_134._data)) {
                return ir_float_type();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Float32>(_match_134._data)) {
                return ir_float32_type();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Str>(_match_134._data)) {
                return ir_string_type();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Bytes>(_match_134._data)) {
                return ir_bytes_type();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Auto>(_match_134._data)) {
                return ir_auto_type();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::NullType>(_match_134._data)) {
                return ir_nullable_type(ir_auto_type());
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::CString>(_match_134._data)) {
                return ir_cstring_type();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Ptr>(_match_134._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_134)>::Ptr>(_match_134._data);
                auto& inner = *_v.inner;
                return ir_ptr_type((*this).lower_type(inner));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Nullable>(_match_134._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_134)>::Nullable>(_match_134._data);
                auto& inner = *_v.inner;
                return ir_nullable_type((*this).lower_type(inner));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Array>(_match_134._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_134)>::Array>(_match_134._data);
                auto& inner = *_v.inner;
                return ir_array_type((*this).lower_type(inner));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::HashSet>(_match_134._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_134)>::HashSet>(_match_134._data);
                auto& inner = *_v.inner;
                return ir_set_type((*this).lower_type(inner));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::HashMap>(_match_134._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_134)>::HashMap>(_match_134._data);
                auto& key_type = *_v.key_type;
                auto& value_type = *_v.value_type;
                return ir_map_type((*this).lower_type(key_type), (*this).lower_type(value_type));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_134)>::Custom>(_match_134._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_134)>::Custom>(_match_134._data);
                auto& name = _v.name;
                auto& type_args = _v.type_args;
                if ((name == std::string("__closure"))) {
                    return ir_generic_closure_type();
                }
                if ((static_cast<int64_t>(type_args.size()) == INT64_C(0))) {
                    return ir_type(name);
                }
                std::vector<std::string> parts = {};
                for (const auto& arg : type_args) {
                    parts.push_back(ir_type_to_string((*this).lower_type(arg)));
                }
                return ir_type(((((std::string("") + (name)) + std::string("<")) + (lv_join(parts, std::string(", ")))) + std::string(">")));
            }
            else if (_match_134._tag == "None") {
                return ir_auto_type();
            }
        }
    }

    IrType infer_binary_type(const LoweredValue& left, const Token& op, const LoweredValue& right) {
        if ((op.token_type == TK_EQUAL_EQUAL) || (op.token_type == TK_BANG_EQUAL) || (op.token_type == TK_LESS) || (op.token_type == TK_LESS_EQUAL) || (op.token_type == TK_GREATER) || (op.token_type == TK_GREATER_EQUAL) || (op.token_type == TK_AND) || (op.token_type == TK_OR)) {
            return ir_bool_type();
        }
        if (ir_type_is_string(left.value_type) || ir_type_is_string(right.value_type)) {
            return ir_string_type();
        }
        if (ir_type_is_float(left.value_type) || ir_type_is_float(right.value_type)) {
            return ir_float_type();
        }
        if (ir_type_is_auto(left.value_type) && (!ir_type_is_auto(right.value_type))) {
            return right.value_type;
        }
        if (ir_type_is_auto(right.value_type) && (!ir_type_is_auto(left.value_type))) {
            return left.value_type;
        }
        return left.value_type;
    }

    std::string next_closure_name() {
        std::string name = ((std::string("__closure_") + (this->closure_counter)) + std::string(""));
        this->closure_counter = (this->closure_counter + INT64_C(1));
        return name;
    }

    std::string next_closure_helper_name() {
        std::string name = ((std::string("__closure_fn_") + (this->closure_counter)) + std::string(""));
        this->closure_counter = (this->closure_counter + INT64_C(1));
        return name;
    }

    IrCallSig lower_typed_call_signature(const TypedExpr& expr) {
        std::vector<IrType> param_types = {};
        for (const auto& param_type : expr.call_signature.param_types) {
            param_types.push_back((*this).lower_type(param_type));
        }
        return ir_call_sig_from_lowered_types(param_types, (*this).lower_type(expr.call_signature.return_type));
    }

    IrClosureSig lower_typed_closure_signature(const TypedExpr& expr) {
        std::vector<IrType> param_types = {};
        for (const auto& param_type : expr.closure_signature.param_types) {
            param_types.push_back((*this).lower_type(param_type));
        }
        return ir_closure_sig_from_lowered_types(param_types, (*this).lower_type(expr.closure_signature.return_type));
    }

    IrCallTarget lower_typed_call_target(const TypedExpr& expr) {
        IrCallSig signature = (*this).lower_typed_call_signature(expr);
        return ir_call_target_from_typed(expr.call_target, signature);
    }

    std::vector<std::string> lower_call_args(const TypedExpr& expr) {
        std::vector<std::string> lowered_args = {};
        for (const auto& arg : expr.children) {
            auto lowered_arg = (*this).lower_expr(arg);
            lowered_args.push_back(lowered_arg.name);
        }
        return lowered_args;
    }

    LoweredValue lower_call_expr(const TypedExpr& expr) {
        std::vector<std::string> lowered_args = (*this).lower_call_args(expr);
        IrCallTarget call_target = (*this).lower_typed_call_target(expr);
        IrType result_type = (*this).lower_type(expr.value_type);
        result_type = ir_resolve_call_result_type(result_type, this->call_return_types, expr.call_target.lowered_name, this->current_return_type);
        if (ir_type_has_auto(result_type)) {
            std::string dest = (*this).next_temp();
            (*this).emit(ir_call_auto(dest, call_target, lowered_args));
            return LoweredValue(dest, ir_auto_type());
        }
        std::string dest = (*this).new_temp(result_type);
        (*this).emit(ir_call(dest, result_type, call_target, lowered_args));
        return LoweredValue(dest, result_type);
    }

    std::vector<std::string> lower_vector_elements_with_hint(const std::vector<TypedExpr>& elements, const IrType& element_hint) {
        std::vector<std::string> lowered_elements = {};
        for (const auto& element : elements) {
            if ((element.kind == std::string("vector")) && (static_cast<int64_t>(element.children.size()) == INT64_C(0)) && (!ir_type_is_auto(element_hint))) {
                std::string empty_name = (*this).new_temp(element_hint);
                std::vector<std::string> empty_items = {};
                (*this).emit(ir_vector_literal(empty_name, element_hint, empty_items));
                lowered_elements.push_back(empty_name);
            }
            else {
                auto lowered_element = (*this).lower_expr(element);
                lowered_elements.push_back(lowered_element.name);
            }
        }
        return lowered_elements;
    }

    void lower_named_binding_init(std::string binding_name, IrType declared_type, const TypedExpr& value_expr) {
        if ((value_expr.kind == std::string("lambda")) || (value_expr.kind == std::string("block_lambda"))) {
            (*this).lower_closure_expr(value_expr, binding_name);
        }
        else {
            if (ir_type_is_auto(declared_type) && (value_expr.kind == std::string("call"))) {
                std::vector<std::string> lowered_args = (*this).lower_call_args(value_expr);
                IrCallTarget call_target = (*this).lower_typed_call_target(value_expr);
                ir_emit_call_auto_named(this->active_blocks[this->current_block].instructions, binding_name, value_expr, lowered_args, call_target);
            }
            else {
                if ((value_expr.kind == std::string("vector"))) {
                    (*this).declare_local(binding_name, declared_type);
                    IrType element_hint = ir_element_type_of(declared_type);
                    std::vector<std::string> lowered_elements = (*this).lower_vector_elements_with_hint(value_expr.children, element_hint);
                    ir_emit_named_vector_literal(this->active_blocks[this->current_block].instructions, binding_name, declared_type, lowered_elements);
                }
                else {
                    if ((value_expr.kind == std::string("map"))) {
                        (*this).declare_local(binding_name, declared_type);
                        std::vector<std::string> lowered_items = {};
                        for (int64_t i = INT64_C(0); i < static_cast<int64_t>(value_expr.children.size()); i++) {
                            auto lowered_key = (*this).lower_expr(value_expr.children[i]);
                            auto lowered_value = (*this).lower_expr(value_expr.extra_children[i]);
                            lowered_items.push_back(lowered_key.name);
                            lowered_items.push_back(lowered_value.name);
                        }
                        ir_emit_named_map_literal(this->active_blocks[this->current_block].instructions, binding_name, declared_type, lowered_items);
                    }
                    else {
                        (*this).declare_local(binding_name, declared_type);
                        auto lowered_value = (*this).lower_expr(value_expr);
                        (*this).emit(ir_move(binding_name, declared_type, lowered_value.name));
                    }
                }
            }
        }
    }

    std::vector<IrParam> closure_capture_params(const TypedExpr& expr) {
        std::vector<IrType> types = {};
        for (int64_t i = INT64_C(0); i < static_cast<int64_t>(expr.capture_names.size()); i++) {
            if ((i < static_cast<int64_t>(expr.closure_signature.capture_types.size()))) {
                types.push_back((*this).lower_type(expr.closure_signature.capture_types[i]));
            }
        }
        return ir_params_from_names_and_types(expr.capture_names, types);
    }

    std::vector<IrParam> closure_lambda_params(const TypedExpr& expr) {
        std::vector<IrType> types = {};
        for (int64_t i = INT64_C(0); i < static_cast<int64_t>(expr.names.size()); i++) {
            if ((i < static_cast<int64_t>(expr.closure_signature.param_types.size()))) {
                types.push_back((*this).lower_type(expr.closure_signature.param_types[i]));
            }
        }
        return ir_params_from_names_and_types(expr.names, types);
    }

    void reset_function_state() {
        this->active_blocks = {};
        this->active_locals = {};
        this->local_types = {{}};
        this->break_targets = {};
        this->continue_targets = {};
    }

    IrFunction lower_closure_helper_expr(std::string helper_name, const TypedExpr& expr, const std::vector<IrParam>& capture_params, const std::vector<IrParam>& lambda_params) {
        std::vector<IrBlock> saved_blocks = this->active_blocks;
        int64_t saved_current_block = this->current_block;
        std::vector<IrLocal> saved_locals = this->active_locals;
        std::unordered_map<std::string, IrType> saved_local_types = this->local_types;
        std::vector<std::string> saved_break_targets = this->break_targets;
        std::vector<std::string> saved_continue_targets = this->continue_targets;
        IrType saved_return_type = this->current_return_type;
        (*this).reset_function_state();
        this->current_return_type = (*this).lower_type(expr.closure_signature.return_type);
        std::vector<IrParam> helper_params = {};
        int64_t entry_index = (*this).create_block(std::string("bb"));
        (*this).switch_to(entry_index);
        for (const auto& param : capture_params) {
            helper_params.push_back(param);
            (*this).declare_local(param.name, param.param_type);
        }
        for (const auto& param : lambda_params) {
            helper_params.push_back(param);
            (*this).declare_local(param.name, param.param_type);
        }
        auto lowered_body = (*this).lower_expr(expr.children[INT64_C(0)]);
        (*this).terminate(ir_term_return_value(lowered_body.name));
        IrFunction helper_fn = IrFunction(helper_name, helper_params, this->current_return_type, this->active_locals, this->active_blocks, false, false);
        this->active_blocks = saved_blocks;
        this->current_block = saved_current_block;
        this->active_locals = saved_locals;
        this->local_types = saved_local_types;
        this->break_targets = saved_break_targets;
        this->continue_targets = saved_continue_targets;
        this->current_return_type = saved_return_type;
        return helper_fn;
    }

    IrFunction lower_closure_helper_block(std::string helper_name, const TypedExpr& expr, const std::vector<IrParam>& capture_params, const std::vector<IrParam>& lambda_params) {
        std::vector<IrBlock> saved_blocks = this->active_blocks;
        int64_t saved_current_block = this->current_block;
        std::vector<IrLocal> saved_locals = this->active_locals;
        std::unordered_map<std::string, IrType> saved_local_types = this->local_types;
        std::vector<std::string> saved_break_targets = this->break_targets;
        std::vector<std::string> saved_continue_targets = this->continue_targets;
        IrType saved_return_type = this->current_return_type;
        (*this).reset_function_state();
        std::vector<TypedStmt> body = {};
        if ((expr.int_a >= INT64_C(0)) && (expr.int_a < static_cast<int64_t>(this->lambda_blocks.size()))) {
            body = this->lambda_blocks[expr.int_a].body;
        }
        this->current_return_type = (*this).lower_type(expr.closure_signature.return_type);
        std::vector<IrParam> helper_params = {};
        int64_t entry_index = (*this).create_block(std::string("bb"));
        (*this).switch_to(entry_index);
        for (const auto& param : capture_params) {
            helper_params.push_back(param);
            (*this).declare_local(param.name, param.param_type);
        }
        for (const auto& param : lambda_params) {
            helper_params.push_back(param);
            (*this).declare_local(param.name, param.param_type);
        }
        for (const auto& stmt : body) {
            if ((*this).block_terminated(this->current_block)) {
                break;
            }
            (*this).lower_stmt(stmt);
        }
        if ((!(*this).block_terminated(this->current_block))) {
            if (ir_type_is_void(this->current_return_type)) {
                (*this).terminate(ir_term_return_void());
            }
            else {
                (*this).terminate(ir_term_unreachable());
            }
        }
        IrFunction helper_fn = IrFunction(helper_name, helper_params, this->current_return_type, this->active_locals, this->active_blocks, false, false);
        this->active_blocks = saved_blocks;
        this->current_block = saved_current_block;
        this->active_locals = saved_locals;
        this->local_types = saved_local_types;
        this->break_targets = saved_break_targets;
        this->continue_targets = saved_continue_targets;
        this->current_return_type = saved_return_type;
        return helper_fn;
    }

    LoweredValue lower_closure_expr(const TypedExpr& expr, std::string preferred_name) {
        std::string closure_name = (*this).next_closure_name();
        IrType closure_type = ir_closure_type(closure_name);
        std::string helper_name = (*this).next_closure_helper_name();
        std::vector<IrParam> capture_params = (*this).closure_capture_params(expr);
        std::vector<IrParam> lambda_params = (*this).closure_lambda_params(expr);
        IrFunction helper_fn;
        if ((expr.kind == std::string("lambda"))) {
            helper_fn = (*this).lower_closure_helper_expr(helper_name, expr, capture_params, lambda_params);
        }
        else {
            helper_fn = (*this).lower_closure_helper_block(helper_name, expr, capture_params, lambda_params);
        }
        this->closure_functions.push_back(helper_fn);
        this->closure_defs.push_back(IrClosureDef(closure_name, helper_name, capture_params, lambda_params, (*this).lower_typed_closure_signature(expr)));
        std::vector<std::string> captures = ir_capture_names(expr);
        std::string dest = preferred_name;
        if ((dest == std::string(""))) {
            dest = (*this).next_temp();
        }
        (*this).emit(ir_make_closure(dest, closure_type, closure_name, captures));
        return LoweredValue(dest, closure_type);
    }

    LoweredValue emit_temp_value(IrType result_type, std::string text_a, std::string text_b, std::string text_c, std::string kind) {
        std::string dest = (*this).new_temp(result_type);
        if ((kind == std::string("unary"))) {
            (*this).emit(ir_unary(dest, result_type, text_a, text_b));
        }
        else {
            if ((kind == std::string("get"))) {
                (*this).emit(ir_get_field(dest, result_type, text_a, text_b));
            }
            else {
                if ((kind == std::string("index"))) {
                    (*this).emit(ir_index(dest, result_type, text_a, text_b));
                }
                else {
                    if ((kind == std::string("cast"))) {
                        (*this).emit(ir_convert(dest, result_type, text_a));
                    }
                }
            }
        }
        return LoweredValue(dest, result_type);
    }

    LoweredValue emit_named_move_value(std::string dest_name, IrType result_type, std::string source_name) {
        (*this).declare_local(dest_name, result_type);
        (*this).emit(ir_move(dest_name, result_type, source_name));
        return LoweredValue(dest_name, result_type);
    }

    LoweredValue emit_temp_binary_value(IrType result_type, std::string op_name, std::string left_name, std::string right_name) {
        std::string dest = (*this).new_temp(result_type);
        (*this).emit(ir_binary(dest, result_type, op_name, left_name, right_name));
        return LoweredValue(dest, result_type);
    }

    LoweredValue emit_temp_const_value(IrType result_type, std::string literal_kind, std::string literal_value) {
        std::string dest = (*this).new_temp(result_type);
        (*this).emit(ir_const(dest, result_type, literal_kind, literal_value));
        return LoweredValue(dest, result_type);
    }

    LoweredValue emit_temp_vector_literal_value(IrType result_type, const std::vector<std::string>& lowered_elements) {
        std::string dest = (*this).new_temp(result_type);
        (*this).emit(ir_vector_literal(dest, result_type, lowered_elements));
        return LoweredValue(dest, result_type);
    }

    LoweredValue emit_temp_map_literal_value(IrType result_type, const std::vector<std::string>& lowered_items) {
        std::string dest = (*this).new_temp(result_type);
        (*this).emit(ir_map_literal(dest, result_type, lowered_items));
        return LoweredValue(dest, result_type);
    }

    LoweredValue lower_expr(const TypedExpr& expr) {
        if ((expr.kind == std::string("literal"))) {
            IrType lowered_type = (*this).lower_type(expr.value_type);
            return (*this).emit_temp_const_value(lowered_type, expr.text_a, expr.text_b);
        }
        else {
            if ((expr.kind == std::string("variable"))) {
                IrType lowered_type = (*this).lower_type(expr.value_type);
                if ((this->local_types.count(expr.text_a) > 0)) {
                    return LoweredValue(expr.text_a, this->local_types[expr.text_a]);
                }
                return LoweredValue(expr.text_a, lowered_type);
            }
            else {
                if ((expr.kind == std::string("assign"))) {
                    auto lowered_value = (*this).lower_expr(expr.children[INT64_C(0)]);
                    IrType result_type = (*this).lower_type(expr.value_type);
                    if (ir_type_is_auto(result_type)) {
                        result_type = lowered_value.value_type;
                    }
                    return (*this).emit_named_move_value(expr.text_a, result_type, lowered_value.name);
                }
                else {
                    if ((expr.kind == std::string("unary"))) {
                        auto lowered_right = (*this).lower_expr(expr.children[INT64_C(0)]);
                        IrType result_type = (*this).lower_type(expr.value_type);
                        if (ir_type_is_auto(result_type)) {
                            result_type = lowered_right.value_type;
                        }
                        return (*this).emit_temp_value(result_type, expr.text_a, lowered_right.name, std::string(""), std::string("unary"));
                    }
                    else {
                        if ((expr.kind == std::string("binary"))) {
                            auto lowered_left = (*this).lower_expr(expr.children[INT64_C(0)]);
                            auto lowered_right = (*this).lower_expr(expr.children[INT64_C(1)]);
                            IrType result_type = (*this).lower_type(expr.value_type);
                            if (ir_type_is_auto(result_type)) {
                                Token fake_op = Token(TK_PLUS, expr.text_a, INT64_C(0), INT64_C(0));
                                result_type = (*this).infer_binary_type(lowered_left, fake_op, lowered_right);
                            }
                            return (*this).emit_temp_binary_value(result_type, expr.text_a, lowered_left.name, lowered_right.name);
                        }
                        else {
                            if ((expr.kind == std::string("logical"))) {
                                auto lowered_left = (*this).lower_expr(expr.children[INT64_C(0)]);
                                auto lowered_right = (*this).lower_expr(expr.children[INT64_C(1)]);
                                return (*this).emit_temp_binary_value(ir_bool_type(), expr.text_a, lowered_left.name, lowered_right.name);
                            }
                            else {
                                if ((expr.kind == std::string("grouping"))) {
                                    return (*this).lower_expr(expr.children[INT64_C(0)]);
                                }
                                else {
                                    if ((expr.kind == std::string("call"))) {
                                        return (*this).lower_call_expr(expr);
                                    }
                                    else {
                                        if ((expr.kind == std::string("get"))) {
                                            auto lowered_object = (*this).lower_expr(expr.children[INT64_C(0)]);
                                            IrType result_type = (*this).lower_type(expr.value_type);
                                            result_type = ir_struct_field_result_type(result_type, this->struct_field_types, lowered_object.value_type, expr.text_a);
                                            return (*this).emit_temp_value(result_type, lowered_object.name, expr.text_a, std::string(""), std::string("get"));
                                        }
                                        else {
                                            if ((expr.kind == std::string("index"))) {
                                                auto lowered_object = (*this).lower_expr(expr.children[INT64_C(0)]);
                                                auto lowered_index = (*this).lower_expr(expr.children[INT64_C(1)]);
                                                IrType result_type = (*this).lower_type(expr.value_type);
                                                if (ir_type_is_auto(result_type)) {
                                                    result_type = ir_element_type_of(lowered_object.value_type);
                                                }
                                                return (*this).emit_temp_value(result_type, lowered_object.name, lowered_index.name, std::string(""), std::string("index"));
                                            }
                                            else {
                                                if ((expr.kind == std::string("index_set"))) {
                                                    auto lowered_object = (*this).lower_expr(expr.children[INT64_C(0)]);
                                                    auto lowered_index = (*this).lower_expr(expr.children[INT64_C(1)]);
                                                    auto lowered_value = (*this).lower_expr(expr.children[INT64_C(2)]);
                                                    (*this).emit(ir_index_set(lowered_object.name, lowered_index.name, lowered_value.name));
                                                    return lowered_value;
                                                }
                                                else {
                                                    if ((expr.kind == std::string("vector"))) {
                                                        IrType result_type = (*this).lower_type(expr.value_type);
                                                        if (ir_type_is_auto(result_type)) {
                                                            result_type = ir_array_type(ir_auto_type());
                                                        }
                                                        std::vector<std::string> lowered_elements = {};
                                                        std::vector<LoweredValue> lowered_values = {};
                                                        if ((static_cast<int64_t>(expr.children.size()) > INT64_C(0))) {
                                                            for (const auto& element : expr.children) {
                                                                if ((element.kind == std::string("vector")) && (static_cast<int64_t>(element.children.size()) == INT64_C(0))) {
                                                                    lowered_values.push_back(LoweredValue(std::string(""), ir_array_type(ir_auto_type())));
                                                                }
                                                                else {
                                                                    lowered_values.push_back((*this).lower_expr(element));
                                                                }
                                                            }
                                                            IrType element_type = ir_element_type_of(result_type);
                                                            if (ir_type_is_auto(element_type)) {
                                                                for (const auto& lowered_value : lowered_values) {
                                                                    if ((!ir_type_is_array_of_auto(lowered_value.value_type)) && (!ir_type_is_auto(lowered_value.value_type))) {
                                                                        element_type = lowered_value.value_type;
                                                                    }
                                                                }
                                                            }
                                                            if (ir_type_is_array_of_auto(result_type)) {
                                                                result_type = ir_array_type(element_type);
                                                            }
                                                            for (const auto& lowered_value : lowered_values) {
                                                                if ((lowered_value.name == std::string(""))) {
                                                                    std::string empty_name = (*this).new_temp(element_type);
                                                                    std::vector<std::string> empty_items = {};
                                                                    (*this).emit(ir_vector_literal(empty_name, element_type, empty_items));
                                                                    lowered_elements.push_back(empty_name);
                                                                }
                                                                else {
                                                                    lowered_elements.push_back(lowered_value.name);
                                                                }
                                                            }
                                                        }
                                                        return (*this).emit_temp_vector_literal_value(result_type, lowered_elements);
                                                    }
                                                    else {
                                                        if ((expr.kind == std::string("map"))) {
                                                            IrType result_type = (*this).lower_type(expr.value_type);
                                                            if (ir_type_is_auto(result_type)) {
                                                                result_type = ir_map_type(ir_auto_type(), ir_auto_type());
                                                            }
                                                            std::vector<std::string> lowered_items = {};
                                                            if ((static_cast<int64_t>(expr.children.size()) > INT64_C(0))) {
                                                                auto first_key = (*this).lower_expr(expr.children[INT64_C(0)]);
                                                                auto first_value = (*this).lower_expr(expr.extra_children[INT64_C(0)]);
                                                                if (ir_type_is_map_of_auto(result_type)) {
                                                                    result_type = ir_map_type(first_key.value_type, first_value.value_type);
                                                                }
                                                                lowered_items.push_back(first_key.name);
                                                                lowered_items.push_back(first_value.name);
                                                                for (int64_t i = INT64_C(1); i < static_cast<int64_t>(expr.children.size()); i++) {
                                                                    auto lowered_key = (*this).lower_expr(expr.children[i]);
                                                                    auto lowered_value = (*this).lower_expr(expr.extra_children[i]);
                                                                    lowered_items.push_back(lowered_key.name);
                                                                    lowered_items.push_back(lowered_value.name);
                                                                }
                                                            }
                                                            return (*this).emit_temp_map_literal_value(result_type, lowered_items);
                                                        }
                                                        else {
                                                            if ((expr.kind == std::string("set"))) {
                                                                auto lowered_value = (*this).lower_expr(expr.children[INT64_C(1)]);
                                                                if ((expr.children[INT64_C(0)].kind == std::string("variable"))) {
                                                                    (*this).emit(ir_set_field(expr.children[INT64_C(0)].text_a, expr.text_a, lowered_value.name));
                                                                    return lowered_value;
                                                                }
                                                                this->errors.push_back(std::string("Unsupported set target in IR lowering"));
                                                                return lowered_value;
                                                            }
                                                            else {
                                                                if ((expr.kind == std::string("cast"))) {
                                                                    auto lowered_value = (*this).lower_expr(expr.children[INT64_C(0)]);
                                                                    IrType result_type = (*this).lower_type(expr.value_type);
                                                                    return (*this).emit_temp_value(result_type, lowered_value.name, std::string(""), std::string(""), std::string("cast"));
                                                                }
                                                                else {
                                                                    if ((expr.kind == std::string("lambda")) || (expr.kind == std::string("block_lambda"))) {
                                                                        return (*this).lower_closure_expr(expr, std::string(""));
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        this->errors.push_back(((std::string("Unsupported typed expression in IR lowering: ") + (expr.kind)) + std::string("")));
        return (*this).emit_temp_const_value(ir_auto_type(), std::string("error"), std::string("\"unsupported_expr\""));
    }

    void lower_stmt(const TypedStmt& stmt) {
        if ((stmt.kind == std::string("none"))) {
            return;
        }
        else {
            if ((stmt.kind == std::string("expr_stmt"))) {
                if ((static_cast<int64_t>(stmt.exprs.size()) == INT64_C(0))) {
                    return;
                }
                if ((stmt.exprs[INT64_C(0)].kind == std::string("call"))) {
                    std::vector<std::string> lowered_args = {};
                    for (const auto& arg : stmt.exprs[INT64_C(0)].children) {
                        auto lowered_arg = (*this).lower_expr(arg);
                        lowered_args.push_back(lowered_arg.name);
                    }
                    IrCallTarget call_target = (*this).lower_typed_call_target(stmt.exprs[INT64_C(0)]);
                    ir_emit_call_void_expr(this->active_blocks[this->current_block].instructions, stmt.exprs[INT64_C(0)], lowered_args, call_target);
                }
                else {
                    (*this).lower_expr(stmt.exprs[INT64_C(0)]);
                }
            }
            else {
                if ((stmt.kind == std::string("let"))) {
                    IrType declared_type = (*this).lower_type(stmt.value_type);
                    if ((static_cast<int64_t>(stmt.exprs.size()) == INT64_C(0))) {
                        (*this).declare_local(stmt.text_a, declared_type);
                        return;
                    }
                    auto initializer_expr = stmt.exprs[INT64_C(0)];
                    if ((initializer_expr.kind == std::string("none"))) {
                        (*this).declare_local(stmt.text_a, declared_type);
                        return;
                    }
                    (*this).lower_named_binding_init(stmt.text_a, declared_type, initializer_expr);
                }
                else {
                    if ((stmt.kind == std::string("const"))) {
                        IrType declared_type = (*this).lower_type(stmt.value_type);
                        if ((static_cast<int64_t>(stmt.exprs.size()) == INT64_C(0))) {
                            (*this).declare_local(stmt.text_a, declared_type);
                            return;
                        }
                        auto value_expr = stmt.exprs[INT64_C(0)];
                        (*this).lower_named_binding_init(stmt.text_a, declared_type, value_expr);
                    }
                    else {
                        if ((stmt.kind == std::string("return"))) {
                            if ((static_cast<int64_t>(stmt.exprs.size()) == INT64_C(0))) {
                                (*this).terminate(ir_term_return_void());
                                return;
                            }
                            if ((stmt.exprs[INT64_C(0)].kind == std::string("none"))) {
                                (*this).terminate(ir_term_return_void());
                            }
                            else {
                                auto lowered_value = (*this).lower_expr(stmt.exprs[INT64_C(0)]);
                                auto return_term = ir_term_return_value(lowered_value.name);
                                ir_emit_return_from_exprs(return_term, stmt.exprs, lowered_value.name);
                                (*this).terminate(return_term);
                            }
                        }
                        else {
                            if ((stmt.kind == std::string("block"))) {
                                for (const auto& nested : stmt.children) {
                                    if ((*this).block_terminated(this->current_block)) {
                                        return;
                                    }
                                    (*this).lower_stmt(nested);
                                }
                            }
                            else {
                                if ((stmt.kind == std::string("namespace"))) {
                                    for (const auto& nested : stmt.children) {
                                        if ((*this).block_terminated(this->current_block)) {
                                            return;
                                        }
                                        (*this).lower_stmt(nested);
                                    }
                                }
                                else {
                                    if ((stmt.kind == std::string("if"))) {
                                        auto lowered_condition = (*this).lower_expr(stmt.exprs[INT64_C(0)]);
                                        int64_t then_index = (*this).create_block(std::string("bb_then"));
                                        int64_t else_index = (*this).create_block(std::string("bb_else"));
                                        int64_t end_index = (*this).create_block(std::string("bb_end"));
                                        auto plan = ir_if_plan(this->active_blocks, then_index, else_index, end_index);
                                        (*this).terminate(ir_term_branch(lowered_condition.name, plan.then_name, plan.else_name));
                                        (*this).switch_to(plan.then_index);
                                        (*this).lower_stmt(stmt.children[INT64_C(0)]);
                                        if ((!(*this).block_terminated(this->current_block))) {
                                            (*this).terminate(ir_term_jump(plan.end_name));
                                        }
                                        (*this).switch_to(plan.else_index);
                                        (*this).lower_stmt(stmt.children[INT64_C(1)]);
                                        if ((!(*this).block_terminated(this->current_block))) {
                                            (*this).terminate(ir_term_jump(plan.end_name));
                                        }
                                        (*this).switch_to(plan.end_index);
                                    }
                                    else {
                                        if ((stmt.kind == std::string("while"))) {
                                            int64_t cond_index = (*this).create_block(std::string("bb_while_cond"));
                                            int64_t body_index = (*this).create_block(std::string("bb_while_body"));
                                            int64_t end_index = (*this).create_block(std::string("bb_while_end"));
                                            auto plan = ir_while_plan(this->active_blocks, cond_index, body_index, end_index);
                                            (*this).terminate(ir_term_jump(plan.cond_name));
                                            (*this).switch_to(plan.cond_index);
                                            auto lowered_condition = (*this).lower_expr(stmt.exprs[INT64_C(0)]);
                                            (*this).terminate(ir_term_branch(lowered_condition.name, plan.body_name, plan.end_name));
                                            ir_push_loop_targets(this->break_targets, this->continue_targets, plan.end_name, plan.cond_name);
                                            (*this).switch_to(plan.body_index);
                                            (*this).lower_stmt(stmt.children[INT64_C(0)]);
                                            if ((!(*this).block_terminated(this->current_block))) {
                                                (*this).terminate(ir_term_jump(plan.cond_name));
                                            }
                                            ir_pop_loop_targets(this->break_targets, this->continue_targets);
                                            (*this).switch_to(plan.end_index);
                                        }
                                        else {
                                            if ((stmt.kind == std::string("for"))) {
                                                if ((stmt.exprs[INT64_C(0)].kind == std::string("range"))) {
                                                    std::string iter_name = stmt.text_a;
                                                    auto lowered_start = (*this).lower_expr(stmt.exprs[INT64_C(0)].children[INT64_C(0)]);
                                                    auto lowered_end = (*this).lower_expr(stmt.exprs[INT64_C(0)].children[INT64_C(1)]);
                                                    IrType iter_type = lowered_start.value_type;
                                                    if (ir_type_is_auto(iter_type)) {
                                                        iter_type = ir_int_type();
                                                    }
                                                    (*this).declare_local(iter_name, iter_type);
                                                    (*this).emit(ir_move(iter_name, iter_type, lowered_start.name));
                                                    int64_t cond_index = (*this).create_block(std::string("bb_for_cond"));
                                                    int64_t body_index = (*this).create_block(std::string("bb_for_body"));
                                                    int64_t step_index = (*this).create_block(std::string("bb_for_step"));
                                                    int64_t end_index = (*this).create_block(std::string("bb_for_end"));
                                                    auto plan = ir_for_plan(this->active_blocks, cond_index, body_index, step_index, end_index);
                                                    (*this).terminate(ir_term_jump(plan.cond_name));
                                                    (*this).switch_to(plan.cond_index);
                                                    std::string cmp_name = (*this).new_temp(ir_bool_type());
                                                    (*this).emit(ir_binary_lt(cmp_name, iter_name, lowered_end.name));
                                                    (*this).terminate(ir_term_branch(cmp_name, plan.body_name, plan.end_name));
                                                    ir_push_loop_targets(this->break_targets, this->continue_targets, plan.end_name, plan.step_name);
                                                    (*this).switch_to(plan.body_index);
                                                    (*this).lower_stmt(stmt.children[INT64_C(0)]);
                                                    if ((!(*this).block_terminated(this->current_block))) {
                                                        (*this).terminate(ir_term_jump(plan.step_name));
                                                    }
                                                    (*this).switch_to(plan.step_index);
                                                    std::string one_name = (*this).new_temp(iter_type);
                                                    (*this).emit(ir_const_int(one_name, iter_type, std::string("1")));
                                                    std::string next_name = (*this).new_temp(iter_type);
                                                    (*this).emit(ir_binary_add(next_name, iter_type, iter_name, one_name));
                                                    (*this).emit(ir_move(iter_name, iter_type, next_name));
                                                    (*this).terminate(ir_term_jump(plan.cond_name));
                                                    ir_pop_loop_targets(this->break_targets, this->continue_targets);
                                                    (*this).switch_to(plan.end_index);
                                                }
                                                else {
                                                    auto collection_value = (*this).lower_expr(stmt.exprs[INT64_C(0)]);
                                                    std::string collection_name = collection_value.name;
                                                    std::string index_name = ((std::string("") + (stmt.text_a)) + std::string("__index"));
                                                    std::string item_name = stmt.text_a;
                                                    IrType index_type = ir_int_type();
                                                    IrType item_type = ir_element_type_of(collection_value.value_type);
                                                    if (ir_type_is_auto(item_type)) {
                                                        item_type = (*this).lower_type(stmt.exprs[INT64_C(0)].value_type);
                                                    }
                                                    (*this).declare_local(index_name, index_type);
                                                    (*this).declare_local(item_name, item_type);
                                                    std::string zero_name = (*this).new_temp(index_type);
                                                    (*this).emit(ir_const_int(zero_name, index_type, std::string("0")));
                                                    (*this).emit(ir_move(index_name, index_type, zero_name));
                                                    int64_t cond_index = (*this).create_block(std::string("bb_for_cond"));
                                                    int64_t body_index = (*this).create_block(std::string("bb_for_body"));
                                                    int64_t step_index = (*this).create_block(std::string("bb_for_step"));
                                                    int64_t end_index = (*this).create_block(std::string("bb_for_end"));
                                                    auto plan = ir_for_plan(this->active_blocks, cond_index, body_index, step_index, end_index);
                                                    (*this).terminate(ir_term_jump(plan.cond_name));
                                                    (*this).switch_to(plan.cond_index);
                                                    std::string len_name = (*this).new_temp(ir_int_type());
                                                    std::vector<std::string> len_args = std::vector{collection_name};
                                                    (*this).emit(ir_call(len_name, ir_int_type(), ir_len_call_target(collection_value.value_type), len_args));
                                                    std::string cmp_name = (*this).new_temp(ir_bool_type());
                                                    (*this).emit(ir_binary_lt(cmp_name, index_name, len_name));
                                                    (*this).terminate(ir_term_branch(cmp_name, plan.body_name, plan.end_name));
                                                    ir_push_loop_targets(this->break_targets, this->continue_targets, plan.end_name, plan.step_name);
                                                    (*this).switch_to(plan.body_index);
                                                    (*this).emit(ir_index(item_name, item_type, collection_name, index_name));
                                                    (*this).lower_stmt(stmt.children[INT64_C(0)]);
                                                    if ((!(*this).block_terminated(this->current_block))) {
                                                        (*this).terminate(ir_term_jump(plan.step_name));
                                                    }
                                                    (*this).switch_to(plan.step_index);
                                                    std::string one_name = (*this).new_temp(index_type);
                                                    (*this).emit(ir_const_int(one_name, index_type, std::string("1")));
                                                    std::string next_name = (*this).new_temp(index_type);
                                                    (*this).emit(ir_binary_add(next_name, index_type, index_name, one_name));
                                                    (*this).emit(ir_move(index_name, index_type, next_name));
                                                    (*this).terminate(ir_term_jump(plan.cond_name));
                                                    ir_pop_loop_targets(this->break_targets, this->continue_targets);
                                                    (*this).switch_to(plan.end_index);
                                                }
                                            }
                                            else {
                                                if ((stmt.kind == std::string("break"))) {
                                                    std::string target = ir_current_break_target(this->break_targets);
                                                    if ((target == std::string(""))) {
                                                        this->errors.push_back(std::string("break used outside loop"));
                                                        return;
                                                    }
                                                    (*this).terminate(ir_term_jump(target));
                                                    int64_t dead_index = (*this).create_block(std::string("bb_dead"));
                                                    (*this).switch_to(dead_index);
                                                }
                                                else {
                                                    if ((stmt.kind == std::string("continue"))) {
                                                        std::string target = ir_current_continue_target(this->continue_targets);
                                                        if ((target == std::string(""))) {
                                                            this->errors.push_back(std::string("continue used outside loop"));
                                                            return;
                                                        }
                                                        (*this).terminate(ir_term_jump(target));
                                                        int64_t dead_index = (*this).create_block(std::string("bb_dead"));
                                                        (*this).switch_to(dead_index);
                                                    }
                                                    else {
                                                        if ((stmt.kind == std::string("pass"))) {
                                                            return;
                                                        }
                                                        else {
                                                            if ((stmt.kind == std::string("try"))) {
                                                                this->errors.push_back(std::string("try is not yet supported in IR lowering"));
                                                            }
                                                            else {
                                                                if ((stmt.kind == std::string("match"))) {
                                                                    auto lowered_match_value = (*this).lower_expr(stmt.exprs[INT64_C(0)]);
                                                                    std::string enum_name = ir_match_enum_name(stmt.exprs[INT64_C(0)].value_type);
                                                                    int64_t end_index = (*this).create_block(std::string("bb_match_end"));
                                                                    std::string end_name = this->active_blocks[end_index].name;
                                                                    int64_t next_index = this->current_block;
                                                                    for (int64_t i = INT64_C(0); i < static_cast<int64_t>(stmt.children.size()); i++) {
                                                                        (*this).switch_to(next_index);
                                                                        std::string pattern_name = stmt.names[i];
                                                                        int64_t body_index = (*this).create_block(std::string("bb_match_arm"));
                                                                        bool is_wildcard = (pattern_name == std::string("_"));
                                                                        int64_t fail_index = end_index;
                                                                        if ((!is_wildcard) && ((i + INT64_C(1)) < static_cast<int64_t>(stmt.children.size()))) {
                                                                            fail_index = (*this).create_block(std::string("bb_match_next"));
                                                                        }
                                                                        auto arm_plan = ir_match_arm_plan(this->active_blocks, body_index, fail_index, is_wildcard);
                                                                        if (is_wildcard) {
                                                                            (*this).terminate(ir_term_jump(arm_plan.body_name));
                                                                        }
                                                                        else {
                                                                            std::string cond_name = (*this).new_temp(ir_bool_type());
                                                                            (*this).emit(ir_enum_is(cond_name, lowered_match_value.name, pattern_name));
                                                                            (*this).terminate(ir_term_branch(cond_name, arm_plan.body_name, arm_plan.fail_name));
                                                                        }
                                                                        (*this).switch_to(body_index);
                                                                        if ((enum_name != std::string(""))) {
                                                                            auto variant = ir_enum_variant_lookup(this->enum_variants, enum_name, pattern_name);
                                                                            for (int64_t bi = INT64_C(0); bi < static_cast<int64_t>(stmt.name_groups[i].size()); bi++) {
                                                                                if ((bi < static_cast<int64_t>(variant.field_names.size())) && (bi < static_cast<int64_t>(variant.types.size()))) {
                                                                                    std::string binding_name = stmt.name_groups[i][bi];
                                                                                    IrType binding_type = (*this).lower_type(variant.types[bi]);
                                                                                    (*this).declare_local(binding_name, binding_type);
                                                                                    (*this).emit(ir_enum_get(binding_name, binding_type, lowered_match_value.name, pattern_name, variant.field_names[bi]));
                                                                                }
                                                                            }
                                                                        }
                                                                        (*this).lower_stmt(stmt.children[i]);
                                                                        if ((!(*this).block_terminated(this->current_block))) {
                                                                            (*this).terminate(ir_term_jump(end_name));
                                                                        }
                                                                        next_index = arm_plan.fail_index;
                                                                    }
                                                                    (*this).switch_to(end_index);
                                                                }
                                                                else {
                                                                    this->errors.push_back(((std::string("Unsupported typed statement kind in IR lowering: ") + (stmt.kind)) + std::string("")));
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    IrFunction lower_typed_function(const TypedFunctionInfo& fn_info) {
        this->active_blocks = {};
        this->active_locals = {};
        this->local_types = {{}};
        this->break_targets = {};
        this->continue_targets = {};
        this->current_return_type = (*this).lower_type(fn_info.return_type);
        std::vector<IrParam> lowered_params = {};
        int64_t entry_index = (*this).create_block(std::string("bb"));
        (*this).switch_to(entry_index);
        for (const auto& param : fn_info.params) {
            IrType param_type = (*this).lower_type(param.param_type);
            lowered_params.push_back(IrParam(param.name.lexeme, param_type));
            (*this).declare_local(param.name.lexeme, param_type);
        }
        for (const auto& body_stmt : fn_info.body) {
            if ((*this).block_terminated(this->current_block)) {
                break;
            }
            (*this).lower_stmt(body_stmt);
        }
        if ((!(*this).block_terminated(this->current_block))) {
            if (ir_type_is_void(this->current_return_type)) {
                (*this).terminate(ir_term_return_void());
            }
            else {
                (*this).terminate(ir_term_unreachable());
            }
        }
        return IrFunction(fn_info.name, lowered_params, this->current_return_type, this->active_locals, this->active_blocks, (fn_info.name == std::string("main")), false);
    }

    IrLowerResult lower_program(const TypedProgram& program) {
        this->struct_field_types = {{}};
        this->call_return_types = {{}};
        this->enum_variants = {{}};
        this->lambda_blocks = program.lambda_blocks;
        for (const auto& struct_info : program.structs) {
            this->call_return_types[struct_info.name] = ir_type(struct_info.name);
            for (const auto& field : struct_info.fields) {
                this->struct_field_types[((((std::string("") + (struct_info.name)) + std::string(".")) + (field.name)) + std::string(""))] = (*this).lower_type(field.field_type);
            }
        }
        for (const auto& enum_info : program.enums) {
            this->call_return_types[enum_info.name] = ir_type(enum_info.name);
            this->enum_variants[enum_info.name] = enum_info.variants;
            for (const auto& variant : enum_info.variants) {
                this->call_return_types[((((std::string("") + (enum_info.name)) + std::string("::make_")) + (variant.name)) + std::string(""))] = ir_type(enum_info.name);
            }
        }
        std::vector<IrFunction> functions = {};
        for (const auto& fn_info : program.functions) {
            this->call_return_types[fn_info.name] = (*this).lower_type(fn_info.return_type);
        }
        for (const auto& fn_info : program.functions) {
            functions.push_back((*this).lower_typed_function(fn_info));
        }
        if ((static_cast<int64_t>(program.top_level.size()) > INT64_C(0))) {
            TypedFunctionInfo top_level_fn = TypedFunctionInfo(item_id((-INT64_C(1))), program.top_level_body_id, std::string("__module_init"), {}, TypeNode::make_Void(), program.top_level);
            functions.push_back((*this).lower_typed_function(top_level_fn));
        }
        std::vector<IrFunction> ordered_functions = {};
        for (const auto& closure_fn : this->closure_functions) {
            ordered_functions.push_back(closure_fn);
        }
        for (const auto& fn_ir : functions) {
            ordered_functions.push_back(fn_ir);
        }
        return IrLowerResult(IrModule(program.name, this->closure_defs, ordered_functions), this->errors);
    }

};

std::vector<std::string> lower_ir_errors(const IrLowerResult& result) {
    return result.errors;
}

IrLowerResult lower_to_ir(const TypedProgram& program) {
    auto lowerer = IrLowerer();
    return lowerer.lower_program(program);
}

struct Parser {
    std::vector<Token> tokens;
    int64_t current;
    bool in_class_body;
    std::vector<std::vector<Stmt>> lambda_blocks;
    std::vector<Expr> last_param_defaults;

    Parser(std::vector<Token> tokens)
        : tokens(tokens) {
        this->current = INT64_C(0);
        this->in_class_body = false;
        this->lambda_blocks = {};
        this->last_param_defaults = {};
    }

    bool is_at_end() {
        return ((*this).peek().token_type == TK_EOF);
    }

    Token peek() {
        return this->tokens[this->current];
    }

    Token peek_at(int64_t offset) {
        if (((this->current + offset) >= static_cast<int64_t>(this->tokens.size()))) {
            return this->tokens[(static_cast<int64_t>(this->tokens.size()) - INT64_C(1))];
        }
        return this->tokens[(this->current + offset)];
    }

    Token previous() {
        return this->tokens[(this->current - INT64_C(1))];
    }

    Token advance() {
        if ((!(*this).is_at_end())) {
            this->current = (this->current + INT64_C(1));
        }
        return (*this).previous();
    }

    Token consume(std::string tk_type, std::string message) {
        if ((*this).check(tk_type)) {
            return (*this).advance();
        }
        auto t = (*this).peek();
        throw std::runtime_error(((((((((std::string("") + (message)) + std::string(" Got ")) + (t.token_type)) + std::string(" at ")) + (t.line)) + std::string(":")) + (t.col)) + std::string("")));
    }

    bool check(std::string tk_type) {
        if ((*this).is_at_end()) {
            return false;
        }
        return ((*this).peek().token_type == tk_type);
    }

    bool match_any(std::vector<std::string> types) {
        for (const auto& t : types) {
            if ((*this).check(t)) {
                (*this).advance();
                return true;
            }
        }
        return false;
    }

    void skip_formatting() {
        while ((*this).match_any(std::vector{TK_NEWLINE, TK_INDENT, TK_DEDENT})) {
            /* pass */
        }
    }

    bool is_type_at_pos(int64_t pos) {
        auto t = (*this).peek_at(pos).token_type;
        std::vector<std::string> type_tokens = std::vector{TK_INT_TYPE, TK_FLOAT_TYPE, TK_STRING_TYPE, TK_BOOL, TK_VOID, TK_AUTO, TK_VECTOR, TK_HASHMAP, TK_HASHSET, TK_IDENTIFIER, TK_INT8, TK_INT16, TK_INT32, TK_INT64, TK_FLOAT32, TK_FLOAT64, TK_USIZE, TK_CSTRING, TK_PTR, TK_BYTES};
        return lv_contains(type_tokens, t);
    }

    int64_t skip_type_tokens(int64_t pos) {
        auto tt = (*this).peek_at(pos).token_type;
        if ((tt == TK_PTR)) {
            pos = (pos + INT64_C(1));
            pos = (pos + INT64_C(1));
            pos = (*this).skip_type_tokens(pos);
            pos = (pos + INT64_C(1));
            return pos;
        }
        if ((tt == TK_VECTOR) || (tt == TK_HASHSET)) {
            pos = (pos + INT64_C(1));
            pos = (pos + INT64_C(1));
            pos = (*this).skip_type_tokens(pos);
            pos = (pos + INT64_C(1));
            return pos;
        }
        else {
            if ((tt == TK_HASHMAP)) {
                pos = (pos + INT64_C(1));
                pos = (pos + INT64_C(1));
                pos = (*this).skip_type_tokens(pos);
                pos = (pos + INT64_C(1));
                pos = (*this).skip_type_tokens(pos);
                pos = (pos + INT64_C(1));
                return pos;
            }
        }
        if ((tt == TK_IDENTIFIER)) {
            int64_t id_pos = (pos + INT64_C(1));
            while (((*this).peek_at(id_pos).token_type == TK_DOUBLE_COLON) && ((*this).peek_at((id_pos + INT64_C(1))).token_type == TK_IDENTIFIER)) {
                id_pos = (id_pos + INT64_C(2));
            }
            if (((*this).peek_at(id_pos).token_type == TK_LEFT_BRACKET)) {
                int64_t try_pos = (id_pos + INT64_C(1));
                try_pos = (*this).skip_type_tokens(try_pos);
                while (((*this).peek_at(try_pos).token_type == TK_COMMA)) {
                    try_pos = (try_pos + INT64_C(1));
                    try_pos = (*this).skip_type_tokens(try_pos);
                }
                if (((*this).peek_at(try_pos).token_type == TK_RIGHT_BRACKET)) {
                    return (try_pos + INT64_C(1));
                }
            }
            return id_pos;
        }
        return (pos + INT64_C(1));
    }

    bool is_function_start() {
        int64_t offset = INT64_C(0);
        if (((*this).peek_at(offset).token_type == TK_COMPTIME) || ((*this).peek_at(offset).token_type == TK_COMPTIME_STRICT)) {
            offset = (offset + INT64_C(1));
        }
        if ((!(*this).is_type_at_pos(offset))) {
            return false;
        }
        offset = (*this).skip_type_tokens(offset);
        if (((*this).peek_at(offset).token_type == TK_QUESTION)) {
            offset = (offset + INT64_C(1));
        }
        bool still_modifiers = true;
        while ((offset < static_cast<int64_t>(this->tokens.size())) && still_modifiers) {
            auto mt = (*this).peek_at(offset).token_type;
            if ((mt == TK_INLINE) || (mt == TK_COMPTIME)) {
                offset = (offset + INT64_C(1));
            }
            else {
                still_modifiers = false;
            }
        }
        auto final_tt = (*this).peek_at(offset).token_type;
        return (final_tt == TK_FN) || (final_tt == TK_OPERATOR);
    }

    TypeNode parse_type() {
        TypeNode t = TypeNode::make_None();
        if ((*this).match_any(std::vector{TK_INT_TYPE})) {
            t = TypeNode::make_Int();
        }
        else {
            if ((*this).match_any(std::vector{TK_FLOAT_TYPE})) {
                t = TypeNode::make_Float();
            }
            else {
                if ((*this).match_any(std::vector{TK_STRING_TYPE})) {
                    t = TypeNode::make_Str();
                }
                else {
                    if ((*this).match_any(std::vector{TK_BOOL})) {
                        t = TypeNode::make_Bool();
                    }
                    else {
                        if ((*this).match_any(std::vector{TK_VOID})) {
                            t = TypeNode::make_Void();
                        }
                        else {
                            if ((*this).match_any(std::vector{TK_AUTO})) {
                                t = TypeNode::make_Auto();
                            }
                            else {
                                if ((*this).match_any(std::vector{TK_NULL})) {
                                    t = TypeNode::make_NullType();
                                }
                                else {
                                    if ((*this).match_any(std::vector{TK_VECTOR})) {
                                        (*this).consume(TK_LEFT_BRACKET, std::string("Expect '[' after 'vector'."));
                                        TypeNode inner = (*this).parse_type();
                                        (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after vector type."));
                                        t = TypeNode::make_Array(inner);
                                    }
                                    else {
                                        if ((*this).match_any(std::vector{TK_HASHSET})) {
                                            (*this).consume(TK_LEFT_BRACKET, std::string("Expect '[' after 'hashset'."));
                                            TypeNode inner = (*this).parse_type();
                                            (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after hashset type."));
                                            t = TypeNode::make_HashSet(inner);
                                        }
                                        else {
                                            if ((*this).match_any(std::vector{TK_HASHMAP})) {
                                                (*this).consume(TK_LEFT_BRACKET, std::string("Expect '[' after 'hashmap'."));
                                                TypeNode k = (*this).parse_type();
                                                (*this).consume(TK_COMMA, std::string("Expect ',' between key and value types."));
                                                TypeNode v = (*this).parse_type();
                                                (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after hashmap types."));
                                                t = TypeNode::make_HashMap(k, v);
                                            }
                                            else {
                                                if ((*this).match_any(std::vector{TK_INT8})) {
                                                    t = TypeNode::make_Int8();
                                                }
                                                else {
                                                    if ((*this).match_any(std::vector{TK_INT16})) {
                                                        t = TypeNode::make_Int16();
                                                    }
                                                    else {
                                                        if ((*this).match_any(std::vector{TK_INT32})) {
                                                            t = TypeNode::make_Int32();
                                                        }
                                                        else {
                                                            if ((*this).match_any(std::vector{TK_INT64})) {
                                                                t = TypeNode::make_Int();
                                                            }
                                                            else {
                                                                if ((*this).match_any(std::vector{TK_FLOAT32})) {
                                                                    t = TypeNode::make_Float32();
                                                                }
                                                                else {
                                                                    if ((*this).match_any(std::vector{TK_FLOAT64})) {
                                                                        t = TypeNode::make_Float();
                                                                    }
                                                                    else {
                                                                        if ((*this).match_any(std::vector{TK_USIZE})) {
                                                                            t = TypeNode::make_USize();
                                                                        }
                                                                        else {
                                                                            if ((*this).match_any(std::vector{TK_CSTRING})) {
                                                                                t = TypeNode::make_CString();
                                                                            }
                                                                            else {
                                                                                if ((*this).match_any(std::vector{TK_BYTES})) {
                                                                                    t = TypeNode::make_Bytes();
                                                                                }
                                                                                else {
                                                                                    if ((*this).match_any(std::vector{TK_PTR})) {
                                                                                        (*this).consume(TK_LEFT_BRACKET, std::string("Expect '[' after 'ptr'."));
                                                                                        TypeNode inner = (*this).parse_type();
                                                                                        (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after ptr type."));
                                                                                        t = TypeNode::make_Ptr(inner);
                                                                                    }
                                                                                    else {
                                                                                        if ((*this).match_any(std::vector{TK_IDENTIFIER})) {
                                                                                            std::string custom_name = (*this).previous().lexeme;
                                                                                            while ((*this).check(TK_DOUBLE_COLON)) {
                                                                                                (*this).advance();
                                                                                                auto part = (*this).consume(TK_IDENTIFIER, std::string("Expect type name after '::'."));
                                                                                                custom_name = ((custom_name + std::string("::")) + part.lexeme);
                                                                                            }
                                                                                            std::vector<TypeNode> type_args = {};
                                                                                            if ((*this).check(TK_LEFT_BRACKET)) {
                                                                                                int64_t save_pos = this->current;
                                                                                                (*this).advance();
                                                                                                bool is_type_args = true;
                                                                                                try {
                                                                                                    TypeNode first_arg = (*this).parse_type();
                                                                                                    type_args.push_back(first_arg);
                                                                                                    while ((*this).match_any(std::vector{TK_COMMA})) {
                                                                                                        if ((*this).check(TK_RIGHT_BRACKET)) {
                                                                                                            break;
                                                                                                        }
                                                                                                        type_args.push_back((*this).parse_type());
                                                                                                    }
                                                                                                    if ((!(*this).check(TK_RIGHT_BRACKET))) {
                                                                                                        is_type_args = false;
                                                                                                    }
                                                                                                }
                                                                                                 catch (const std::exception& e) {
                                                                                                    is_type_args = false;
                                                                                                }
                                                                                                if (is_type_args) {
                                                                                                    (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after type arguments."));
                                                                                                }
                                                                                                else {
                                                                                                    type_args = {};
                                                                                                    this->current = save_pos;
                                                                                                }
                                                                                            }
                                                                                            t = TypeNode::make_Custom(custom_name, type_args);
                                                                                        }
                                                                                        else {
                                                                                            auto tok = (*this).peek();
                                                                                            throw std::runtime_error(((((((std::string("Expect type. Got ") + (tok.token_type)) + std::string(" at ")) + (tok.line)) + std::string(":")) + (tok.col)) + std::string("")));
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if ((*this).match_any(std::vector{TK_QUESTION})) {
            t = TypeNode::make_Nullable(t);
        }
        return t;
    }

    Expr expression() {
        return (*this).assignment();
    }

    Expr assignment() {
        Expr expr = (*this).logical_or();
        if ((*this).match_any(std::vector{TK_EQUAL})) {
            Expr value = (*this).assignment();
            {
                const auto& _match_135 = expr;
                if (std::holds_alternative<std::decay_t<decltype(_match_135)>::Variable>(_match_135._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_135)>::Variable>(_match_135._data);
                    auto& name = _v.name;
                    return Expr::make_Assign(name, value);
                }
                else if (std::holds_alternative<std::decay_t<decltype(_match_135)>::Get>(_match_135._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_135)>::Get>(_match_135._data);
                    auto& object = *_v.object;
                    auto& name = _v.name;
                    return Expr::make_Set(object, name, value);
                }
                else if (std::holds_alternative<std::decay_t<decltype(_match_135)>::Index>(_match_135._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_135)>::Index>(_match_135._data);
                    auto& object = *_v.object;
                    auto& bracket = _v.bracket;
                    auto& index = *_v.index;
                    return Expr::make_IndexSet(object, bracket, index, value);
                }
                else {
                    throw std::runtime_error(std::string("Invalid assignment target."));
                }
            }
        }
        else {
            if ((*this).match_any(std::vector{TK_PLUS_EQUAL, TK_MINUS_EQUAL, TK_STAR_EQUAL})) {
                auto compound_op = (*this).previous();
                Expr rhs = (*this).assignment();
                std::string base_type = TK_PLUS;
                std::string base_lexeme = std::string("+");
                if ((compound_op.token_type == TK_MINUS_EQUAL)) {
                    base_type = TK_MINUS;
                    base_lexeme = std::string("-");
                }
                else {
                    if ((compound_op.token_type == TK_STAR_EQUAL)) {
                        base_type = TK_STAR;
                        base_lexeme = std::string("*");
                    }
                }
                auto op_token = Token(base_type, base_lexeme, compound_op.line, compound_op.col);
                {
                    const auto& _match_136 = expr;
                    if (std::holds_alternative<std::decay_t<decltype(_match_136)>::Variable>(_match_136._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_136)>::Variable>(_match_136._data);
                        auto& name = _v.name;
                        Expr bin = Expr::make_Binary(Expr::make_Variable(name), op_token, rhs);
                        return Expr::make_Assign(name, bin);
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_136)>::Get>(_match_136._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_136)>::Get>(_match_136._data);
                        auto& object = *_v.object;
                        auto& name = _v.name;
                        Expr bin = Expr::make_Binary(Expr::make_Get(object, name), op_token, rhs);
                        return Expr::make_Set(object, name, bin);
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_136)>::Index>(_match_136._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_136)>::Index>(_match_136._data);
                        auto& object = *_v.object;
                        auto& bracket = _v.bracket;
                        auto& index = *_v.index;
                        Expr bin = Expr::make_Binary(Expr::make_Index(object, bracket, index), op_token, rhs);
                        return Expr::make_IndexSet(object, bracket, index, bin);
                    }
                    else {
                        throw std::runtime_error(std::string("Invalid compound assignment target."));
                    }
                }
            }
        }
        return expr;
    }

    Expr logical_or() {
        Expr expr = (*this).logical_and();
        while ((*this).match_any(std::vector{TK_OR})) {
            auto op = (*this).previous();
            Expr right = (*this).logical_and();
            expr = Expr::make_Logical(expr, op, right);
        }
        return expr;
    }

    Expr logical_and() {
        Expr expr = (*this).equality();
        while ((*this).match_any(std::vector{TK_AND})) {
            auto op = (*this).previous();
            Expr right = (*this).equality();
            expr = Expr::make_Logical(expr, op, right);
        }
        return expr;
    }

    Expr equality() {
        Expr expr = (*this).comparison();
        while ((*this).match_any(std::vector{TK_BANG_EQUAL, TK_EQUAL_EQUAL})) {
            auto op = (*this).previous();
            Expr right = (*this).comparison();
            expr = Expr::make_Binary(expr, op, right);
        }
        return expr;
    }

    Expr comparison() {
        Expr expr = (*this).term();
        while ((*this).match_any(std::vector{TK_GREATER, TK_GREATER_EQUAL, TK_LESS, TK_LESS_EQUAL})) {
            auto op = (*this).previous();
            Expr right = (*this).term();
            expr = Expr::make_Binary(expr, op, right);
        }
        return expr;
    }

    Expr term() {
        Expr expr = (*this).factor();
        while ((*this).match_any(std::vector{TK_MINUS, TK_PLUS})) {
            auto op = (*this).previous();
            Expr right = (*this).factor();
            expr = Expr::make_Binary(expr, op, right);
        }
        return expr;
    }

    Expr factor() {
        Expr expr = (*this).unary();
        while ((*this).match_any(std::vector{TK_SLASH, TK_STAR, TK_PERCENT})) {
            auto op = (*this).previous();
            Expr right = (*this).unary();
            expr = Expr::make_Binary(expr, op, right);
        }
        return expr;
    }

    Expr unary() {
        if ((*this).match_any(std::vector{TK_BANG, TK_MINUS, TK_NOT})) {
            auto op = (*this).previous();
            Expr right = (*this).unary();
            return Expr::make_Unary(op, right);
        }
        if ((*this).match_any(std::vector{TK_OWN})) {
            Expr right = (*this).unary();
            return Expr::make_Own(right);
        }
        if ((*this).match_any(std::vector{TK_AMPERSAND})) {
            Expr right = (*this).unary();
            return Expr::make_AddressOf(right);
        }
        return (*this).call();
    }

    Expr call() {
        Expr expr = (*this).primary();
        {
            const auto& _match_137 = expr;
            if (std::holds_alternative<std::decay_t<decltype(_match_137)>::Variable>(_match_137._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_137)>::Variable>(_match_137._data);
                auto& tok = _v.name;
                if ((*this).check(TK_LEFT_BRACKET)) {
                    int64_t save_pos = this->current;
                    (*this).advance();
                    bool is_type_args = true;
                    std::vector<std::string> type_strs = {};
                    try {
                        TypeNode first_t = (*this).parse_type();
                        type_strs.push_back(type_to_cpp(first_t));
                        while ((*this).match_any(std::vector{TK_COMMA})) {
                            if ((*this).check(TK_RIGHT_BRACKET)) {
                                break;
                            }
                            TypeNode next_t = (*this).parse_type();
                            type_strs.push_back(type_to_cpp(next_t));
                        }
                        if ((!(*this).check(TK_RIGHT_BRACKET))) {
                            is_type_args = false;
                        }
                    }
                     catch (const std::exception& e) {
                        is_type_args = false;
                    }
                    if (is_type_args) {
                        (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']'."));
                        if ((*this).check(TK_LEFT_PAREN) || (*this).check(TK_DOUBLE_COLON)) {
                            std::string new_name = ((((std::string("") + (tok.lexeme)) + std::string("<")) + (lv_join(type_strs, std::string(", ")))) + std::string(">"));
                            expr = Expr::make_Variable(Token(TK_IDENTIFIER, new_name, tok.line, tok.col));
                        }
                        else {
                            this->current = save_pos;
                        }
                    }
                    else {
                        this->current = save_pos;
                    }
                }
            }
            else {
                /* pass */
            }
        }
        bool more = true;
        while (more) {
            if ((*this).match_any(std::vector{TK_LEFT_PAREN})) {
                expr = (*this).finish_call(expr);
            }
            else {
                if ((*this).match_any(std::vector{TK_LEFT_BRACKET})) {
                    Expr index = (*this).expression();
                    auto bracket = (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after index."));
                    expr = Expr::make_Index(expr, bracket, index);
                }
                else {
                    if ((*this).match_any(std::vector{TK_DOT})) {
                        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect property name after '.'."));
                        expr = Expr::make_Get(expr, name);
                    }
                    else {
                        if ((*this).match_any(std::vector{TK_DOUBLE_COLON})) {
                            auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect member name after '::'."));
                            expr = Expr::make_StaticGet(expr, name);
                        }
                        else {
                            if ((*this).match_any(std::vector{TK_AS})) {
                                TypeNode target = (*this).parse_type();
                                expr = Expr::make_Cast(expr, target);
                            }
                            else {
                                more = false;
                            }
                        }
                    }
                }
            }
        }
        return expr;
    }

    Expr finish_call(const Expr& callee) {
        {
            const auto& _match_138 = callee;
            if (std::holds_alternative<std::decay_t<decltype(_match_138)>::Variable>(_match_138._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_138)>::Variable>(_match_138._data);
                auto& name = _v.name;
                if ((name.lexeme == std::string("cast"))) {
                    Expr expr = (*this).expression();
                    (*this).consume(TK_COMMA, std::string("Expect ',' after value in cast()."));
                    TypeNode target = (*this).parse_type();
                    (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after type in cast()."));
                    return Expr::make_Cast(expr, target);
                }
            }
            else {
                /* pass */
            }
        }
        std::vector<Expr> args = {};
        std::vector<std::string> arg_names = {};
        (*this).skip_formatting();
        if ((!(*this).check(TK_RIGHT_PAREN))) {
            (*this).match_any(std::vector{TK_REF, TK_REF_MUT});
            Expr arg_expr = (*this).expression();
            {
                const auto& _match_139 = arg_expr;
                if (std::holds_alternative<std::decay_t<decltype(_match_139)>::Assign>(_match_139._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_139)>::Assign>(_match_139._data);
                    auto& aname = _v.name;
                    auto& avalue = *_v.value;
                    arg_names.push_back(aname.lexeme);
                    args.push_back(avalue);
                }
                else {
                    arg_names.push_back(std::string(""));
                    args.push_back(arg_expr);
                }
            }
            (*this).skip_formatting();
            while ((*this).match_any(std::vector{TK_COMMA})) {
                (*this).skip_formatting();
                if ((*this).check(TK_RIGHT_PAREN)) {
                    break;
                }
                (*this).match_any(std::vector{TK_REF, TK_REF_MUT});
                arg_expr = (*this).expression();
                {
                    const auto& _match_140 = arg_expr;
                    if (std::holds_alternative<std::decay_t<decltype(_match_140)>::Assign>(_match_140._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_140)>::Assign>(_match_140._data);
                        auto& aname = _v.name;
                        auto& avalue = *_v.value;
                        arg_names.push_back(aname.lexeme);
                        args.push_back(avalue);
                    }
                    else {
                        arg_names.push_back(std::string(""));
                        args.push_back(arg_expr);
                    }
                }
                (*this).skip_formatting();
            }
        }
        (*this).skip_formatting();
        auto paren = (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after arguments."));
        return Expr::make_Call(callee, paren, args, arg_names);
    }

    Expr primary() {
        if ((*this).match_any(std::vector{TK_FALSE})) {
            return Expr::make_Literal(std::string("bool"), std::string("false"));
        }
        if ((*this).match_any(std::vector{TK_TRUE})) {
            return Expr::make_Literal(std::string("bool"), std::string("true"));
        }
        if ((*this).match_any(std::vector{TK_NULL})) {
            return Expr::make_Literal(std::string("null"), std::string("null"));
        }
        if ((*this).match_any(std::vector{TK_INT})) {
            return Expr::make_Literal(std::string("int"), (*this).previous().lexeme);
        }
        if ((*this).match_any(std::vector{TK_FLOAT})) {
            return Expr::make_Literal(std::string("float"), (*this).previous().lexeme);
        }
        if ((*this).match_any(std::vector{TK_STRING})) {
            return Expr::make_Literal(std::string("string"), (*this).previous().lexeme);
        }
        if ((*this).match_any(std::vector{TK_IDENTIFIER})) {
            return Expr::make_Variable((*this).previous());
        }
        if ((*this).check(TK_BYTES) && ((*this).peek_at(INT64_C(1)).token_type == TK_DOUBLE_COLON)) {
            auto tok = (*this).advance();
            return Expr::make_Variable(Token(TK_IDENTIFIER, tok.lexeme, tok.line, tok.col));
        }
        if ((*this).match_any(std::vector{TK_THIS})) {
            return Expr::make_This((*this).previous());
        }
        if ((*this).match_any(std::vector{TK_LEFT_PAREN})) {
            if ((*this).try_parse_lambda()) {
                return (*this).parse_lambda();
            }
            (*this).skip_formatting();
            Expr expr = (*this).expression();
            (*this).skip_formatting();
            (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after expression."));
            return Expr::make_Grouping(expr);
        }
        if ((*this).match_any(std::vector{TK_LEFT_BRACKET})) {
            std::vector<Expr> elements = {};
            (*this).skip_formatting();
            if ((!(*this).check(TK_RIGHT_BRACKET))) {
                (*this).skip_formatting();
                elements.push_back((*this).expression());
                (*this).skip_formatting();
                while ((*this).match_any(std::vector{TK_COMMA})) {
                    (*this).skip_formatting();
                    if ((*this).check(TK_RIGHT_BRACKET)) {
                        break;
                    }
                    elements.push_back((*this).expression());
                    (*this).skip_formatting();
                }
            }
            (*this).skip_formatting();
            (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after vector literal."));
            return Expr::make_Vector(elements);
        }
        if ((*this).match_any(std::vector{TK_LEFT_BRACE})) {
            std::vector<Expr> keys = {};
            std::vector<Expr> values = {};
            (*this).skip_formatting();
            if ((!(*this).check(TK_RIGHT_BRACE))) {
                (*this).skip_formatting();
                Expr key = (*this).expression();
                (*this).skip_formatting();
                (*this).consume(TK_COLON, std::string("Expect ':' after map key."));
                (*this).skip_formatting();
                Expr value = (*this).expression();
                (*this).skip_formatting();
                keys.push_back(key);
                values.push_back(value);
                while ((*this).match_any(std::vector{TK_COMMA})) {
                    (*this).skip_formatting();
                    if ((*this).check(TK_RIGHT_BRACE)) {
                        break;
                    }
                    key = (*this).expression();
                    (*this).skip_formatting();
                    (*this).consume(TK_COLON, std::string("Expect ':' after map key."));
                    (*this).skip_formatting();
                    value = (*this).expression();
                    (*this).skip_formatting();
                    keys.push_back(key);
                    values.push_back(value);
                }
            }
            (*this).skip_formatting();
            (*this).consume(TK_RIGHT_BRACE, std::string("Expect '}' after map literal."));
            return Expr::make_Map(keys, values);
        }
        auto t = (*this).peek();
        throw std::runtime_error(((((((std::string("Expect expression. Got ") + (t.token_type)) + std::string(" at ")) + (t.line)) + std::string(":")) + (t.col)) + std::string("")));
    }

    std::vector<std::string> parse_type_params() {
        std::vector<std::string> type_params = {};
        if ((*this).match_any(std::vector{TK_LEFT_BRACKET})) {
            type_params.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect type parameter name.")).lexeme);
            while ((*this).match_any(std::vector{TK_COMMA})) {
                if ((*this).check(TK_RIGHT_BRACKET)) {
                    break;
                }
                type_params.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect type parameter name.")).lexeme);
            }
            (*this).consume(TK_RIGHT_BRACKET, std::string("Expect ']' after type parameters."));
        }
        return type_params;
    }

    std::vector<Param> parse_param_list() {
        std::vector<Param> params = {};
        this->last_param_defaults = {};
        (*this).skip_formatting();
        if ((!(*this).check(TK_RIGHT_PAREN))) {
            bool p_mut = (*this).match_any(std::vector{TK_REF_MUT});
            bool p_ref = p_mut || (*this).match_any(std::vector{TK_REF});
            TypeNode param_type = (*this).parse_type();
            auto param_name = (*this).consume(TK_IDENTIFIER, std::string("Expect parameter name."));
            Expr default_val = Expr::make_None();
            if ((*this).match_any(std::vector{TK_EQUAL})) {
                default_val = (*this).expression();
            }
            params.push_back(Param(param_name, param_type, p_ref, p_mut));
            this->last_param_defaults.push_back(default_val);
            (*this).skip_formatting();
            while ((*this).match_any(std::vector{TK_COMMA})) {
                (*this).skip_formatting();
                if ((*this).check(TK_RIGHT_PAREN)) {
                    break;
                }
                p_mut = (*this).match_any(std::vector{TK_REF_MUT});
                p_ref = p_mut || (*this).match_any(std::vector{TK_REF});
                param_type = (*this).parse_type();
                param_name = (*this).consume(TK_IDENTIFIER, std::string("Expect parameter name."));
                default_val = Expr::make_None();
                if ((*this).match_any(std::vector{TK_EQUAL})) {
                    default_val = (*this).expression();
                }
                params.push_back(Param(param_name, param_type, p_ref, p_mut));
                this->last_param_defaults.push_back(default_val);
                (*this).skip_formatting();
            }
        }
        (*this).skip_formatting();
        return params;
    }

    bool try_parse_lambda() {
        int64_t saved = this->current;
        if ((*this).check(TK_RIGHT_PAREN)) {
            std::string next_type = (*this).peek_at(INT64_C(1)).token_type;
            if ((next_type == TK_FAT_ARROW) || (next_type == TK_COLON)) {
                return true;
            }
        }
        int64_t start_pos = INT64_C(0);
        if (((*this).peek_at(INT64_C(0)).token_type == TK_REF) || ((*this).peek_at(INT64_C(0)).token_type == TK_REF_MUT)) {
            start_pos = INT64_C(1);
        }
        if ((*this).is_type_at_pos(start_pos)) {
            int64_t pos = start_pos;
            bool valid = true;
            while (valid) {
                pos = (*this).skip_type_tokens(pos);
                if (((*this).peek_at(pos).token_type == TK_QUESTION)) {
                    pos = (pos + INT64_C(1));
                }
                if (((*this).peek_at(pos).token_type != TK_IDENTIFIER)) {
                    valid = false;
                }
                else {
                    pos = (pos + INT64_C(1));
                    if (((*this).peek_at(pos).token_type == TK_COMMA)) {
                        pos = (pos + INT64_C(1));
                        if (((*this).peek_at(pos).token_type == TK_REF) || ((*this).peek_at(pos).token_type == TK_REF_MUT)) {
                            pos = (pos + INT64_C(1));
                        }
                        if ((!(*this).is_type_at_pos(pos))) {
                            valid = false;
                        }
                    }
                    else {
                        if (((*this).peek_at(pos).token_type == TK_RIGHT_PAREN)) {
                            std::string after_paren = (*this).peek_at((pos + INT64_C(1))).token_type;
                            if ((after_paren == TK_FAT_ARROW) || (after_paren == TK_COLON)) {
                                return true;
                            }
                            return false;
                        }
                        else {
                            valid = false;
                        }
                    }
                }
            }
        }
        return false;
    }

    Expr parse_lambda() {
        std::vector<Param> params = (*this).parse_param_list();
        (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after lambda parameters."));
        if ((*this).match_any(std::vector{TK_FAT_ARROW})) {
            Expr body = (*this).expression();
            return Expr::make_Lambda(params, body);
        }
        (*this).consume(TK_COLON, std::string("Expect '=>' or ':' after lambda parameters."));
        std::vector<Stmt> body = (*this).block();
        int64_t body_id = static_cast<int64_t>(this->lambda_blocks.size());
        this->lambda_blocks.push_back(body);
        return Expr::make_BlockLambda(params, body_id);
    }

    Stmt statement() {
        if ((*this).match_any(std::vector{TK_IF})) {
            return (*this).if_statement();
        }
        if ((*this).match_any(std::vector{TK_WHILE})) {
            return (*this).while_statement();
        }
        if ((*this).match_any(std::vector{TK_FOR})) {
            return (*this).for_statement();
        }
        if ((*this).match_any(std::vector{TK_RETURN})) {
            return (*this).return_statement();
        }
        if ((*this).match_any(std::vector{TK_THROW})) {
            return (*this).throw_statement();
        }
        if ((*this).match_any(std::vector{TK_MATCH})) {
            return (*this).match_statement();
        }
        if ((*this).match_any(std::vector{TK_BREAK})) {
            auto kw = (*this).previous();
            (*this).match_any(std::vector{TK_NEWLINE});
            return Stmt::make_Break(kw);
        }
        if ((*this).match_any(std::vector{TK_CONTINUE})) {
            auto kw = (*this).previous();
            (*this).match_any(std::vector{TK_NEWLINE});
            return Stmt::make_Continue(kw);
        }
        if ((*this).match_any(std::vector{TK_PASS})) {
            auto kw = (*this).previous();
            (*this).match_any(std::vector{TK_NEWLINE});
            return Stmt::make_Pass(kw);
        }
        if ((*this).match_any(std::vector{TK_CPP})) {
            auto code = (*this).previous().lexeme;
            (*this).match_any(std::vector{TK_NEWLINE});
            return Stmt::make_CppBlock(code);
        }
        return (*this).expression_statement();
    }

    Stmt throw_statement() {
        Expr expr = (*this).expression();
        (*this).match_any(std::vector{TK_SEMICOLON, TK_NEWLINE});
        return Stmt::make_ExprStmt(Expr::make_Throw(expr));
    }

    Stmt if_statement() {
        Expr condition = (*this).expression();
        (*this).consume(TK_COLON, std::string("Expect ':' after if condition."));
        Stmt then_branch = Stmt::make_Block((*this).block());
        Stmt else_branch = Stmt::make_None();
        if ((*this).match_any(std::vector{TK_ELIF})) {
            else_branch = (*this).if_statement();
        }
        else {
            if ((*this).match_any(std::vector{TK_ELSE})) {
                if ((*this).match_any(std::vector{TK_COLON})) {
                    else_branch = Stmt::make_Block((*this).block());
                }
                else {
                    if ((*this).check(TK_IF)) {
                        else_branch = (*this).declaration();
                    }
                }
            }
        }
        return Stmt::make_If(condition, then_branch, else_branch);
    }

    Stmt while_statement() {
        Expr condition = (*this).expression();
        (*this).consume(TK_COLON, std::string("Expect ':' after while condition."));
        Stmt body = Stmt::make_Block((*this).block());
        return Stmt::make_While(condition, body);
    }

    Stmt for_statement() {
        bool f_mut = (*this).match_any(std::vector{TK_REF_MUT});
        bool f_ref = f_mut || (*this).match_any(std::vector{TK_REF});
        auto item_name = (*this).consume(TK_IDENTIFIER, std::string("Expect variable name after 'for'."));
        (*this).consume(TK_IN, std::string("Expect 'in' after variable name."));
        Expr collection = (*this).expression();
        if ((*this).match_any(std::vector{TK_DOT_DOT})) {
            Expr end_expr = (*this).expression();
            collection = Expr::make_Range(collection, end_expr);
        }
        (*this).consume(TK_COLON, std::string("Expect ':' after for loop header."));
        Stmt body = Stmt::make_Block((*this).block());
        return Stmt::make_For(item_name, collection, body, f_ref, f_mut);
    }

    Stmt return_statement() {
        auto keyword = (*this).previous();
        Expr value = Expr::make_None();
        if ((!(*this).check(TK_NEWLINE)) && (!(*this).check(TK_DEDENT)) && (!(*this).is_at_end())) {
            value = (*this).expression();
        }
        (*this).match_any(std::vector{TK_NEWLINE});
        return Stmt::make_Return(keyword, value);
    }

    Stmt match_statement() {
        Expr expr = (*this).expression();
        (*this).consume(TK_COLON, std::string("Expect ':' after match expression."));
        (*this).match_any(std::vector{TK_NEWLINE});
        (*this).consume(TK_INDENT, std::string("Expect indentation to start match body."));
        std::vector<MatchArm> arm_patterns = {};
        std::vector<Stmt> arm_bodies = {};
        while ((!(*this).check(TK_DEDENT)) && (!(*this).is_at_end())) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                /* pass */
            }
            else {
                auto pattern_tok = (*this).consume(TK_IDENTIFIER, std::string("Expect pattern name in match arm."));
                std::string pattern_name = pattern_tok.lexeme;
                std::vector<std::string> bindings = {};
                if ((pattern_name != std::string("_"))) {
                    if ((*this).match_any(std::vector{TK_LEFT_PAREN})) {
                        if ((!(*this).check(TK_RIGHT_PAREN))) {
                            bindings.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect binding name.")).lexeme);
                            while ((*this).match_any(std::vector{TK_COMMA})) {
                                if ((*this).check(TK_RIGHT_PAREN)) {
                                    break;
                                }
                                bindings.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect binding name.")).lexeme);
                            }
                        }
                        (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after bindings."));
                    }
                }
                (*this).consume(TK_COLON, std::string("Expect ':' after match pattern."));
                std::vector<Stmt> body = (*this).block();
                arm_patterns.push_back(MatchArm(pattern_name, bindings));
                arm_bodies.push_back(Stmt::make_Block(body));
            }
        }
        (*this).consume(TK_DEDENT, std::string("Expect dedent to end match body."));
        return Stmt::make_Match(expr, arm_patterns, arm_bodies);
    }

    std::vector<Stmt> block() {
        (*this).match_any(std::vector{TK_NEWLINE});
        (*this).consume(TK_INDENT, std::string("Expect indentation to start a block."));
        std::vector<Stmt> statements = {};
        while ((!(*this).check(TK_DEDENT)) && (!(*this).is_at_end())) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                /* pass */
            }
            else {
                statements.push_back((*this).declaration());
            }
        }
        (*this).consume(TK_DEDENT, std::string("Expect dedent to end a block."));
        return statements;
    }

    Stmt expression_statement() {
        Expr expr = (*this).expression();
        (*this).match_any(std::vector{TK_SEMICOLON, TK_NEWLINE});
        return Stmt::make_ExprStmt(expr);
    }

    Stmt var_declaration(std::string visibility) {
        return (*this).var_declaration_with_ref(visibility, false, false);
    }

    Stmt var_declaration_with_ref(std::string visibility, bool is_ref, bool is_mut) {
        TypeNode var_type = (*this).parse_type();
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect variable name after type."));
        Expr initializer = Expr::make_None();
        if ((*this).match_any(std::vector{TK_EQUAL})) {
            initializer = (*this).expression();
        }
        (*this).match_any(std::vector{TK_SEMICOLON, TK_NEWLINE});
        return Stmt::make_Let(name, var_type, initializer, visibility, is_ref, is_mut);
    }

    Stmt const_declaration(std::string visibility, int64_t comptime_mode) {
        TypeNode const_type = (*this).parse_type();
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect constant name after type."));
        (*this).consume(TK_EQUAL, std::string("Const declaration must have an initializer."));
        Expr value = (*this).expression();
        (*this).match_any(std::vector{TK_SEMICOLON, TK_NEWLINE});
        return Stmt::make_Const(name, const_type, value, visibility, comptime_mode);
    }

    Stmt function_declaration(std::string visibility, bool is_static, int64_t comptime_mode) {
        TypeNode return_type = (*this).parse_type();
        bool is_inline = false;
        while ((*this).match_any(std::vector{TK_INLINE})) {
            is_inline = true;
        }
        Token name = Token(std::string(""), std::string(""), INT64_C(0), INT64_C(0));
        bool is_operator = false;
        if ((*this).match_any(std::vector{TK_OPERATOR})) {
            is_operator = true;
            auto op_tok = (*this).advance();
            name = Token(TK_IDENTIFIER, ((std::string("operator") + (op_tok.lexeme)) + std::string("")), op_tok.line, op_tok.col);
        }
        else {
            (*this).consume(TK_FN, std::string("Expect 'fn' keyword after return type."));
            name = (*this).consume(TK_IDENTIFIER, std::string("Expect function name."));
        }
        std::vector<std::string> type_params = (*this).parse_type_params();
        (*this).consume(TK_LEFT_PAREN, std::string("Expect '(' after function name."));
        std::vector<Param> params = (*this).parse_param_list();
        std::vector<Expr> defaults = this->last_param_defaults;
        (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after parameters."));
        (*this).consume(TK_COLON, std::string("Expect ':' before function body."));
        std::vector<Stmt> body = (*this).block();
        return Stmt::make_Function(name, params, return_type, body, is_inline, comptime_mode, is_static, visibility, type_params, defaults);
    }

    Stmt class_declaration(std::string visibility) {
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect class name."));
        (*this).consume(TK_COLON, std::string("Expect ':' after class name."));
        auto old = this->in_class_body;
        this->in_class_body = true;
        auto body = (*this).block();
        this->in_class_body = old;
        return Stmt::make_Class(name, body, visibility);
    }

    Stmt struct_declaration(std::string visibility) {
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect struct name."));
        std::vector<std::string> type_params = (*this).parse_type_params();
        (*this).consume(TK_COLON, std::string("Expect ':' after struct name."));
        auto old = this->in_class_body;
        this->in_class_body = true;
        auto body = (*this).block();
        this->in_class_body = old;
        return Stmt::make_Struct(name, body, visibility, type_params);
    }

    Stmt enum_declaration(std::string visibility) {
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect enum name."));
        std::vector<std::string> type_params = (*this).parse_type_params();
        (*this).consume(TK_COLON, std::string("Expect ':' after enum name."));
        (*this).match_any(std::vector{TK_NEWLINE});
        (*this).consume(TK_INDENT, std::string("Expect indentation to start enum body."));
        std::vector<EnumVariantNode> variants = {};
        std::vector<Stmt> methods = {};
        while ((!(*this).check(TK_DEDENT)) && (!(*this).is_at_end())) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                /* pass */
            }
            else {
                if ((*this).is_function_start()) {
                    auto old = this->in_class_body;
                    this->in_class_body = true;
                    int64_t method_ct = INT64_C(0);
                    if ((*this).match_any(std::vector{TK_COMPTIME_STRICT})) {
                        method_ct = INT64_C(2);
                    }
                    else {
                        if ((*this).match_any(std::vector{TK_COMPTIME})) {
                            method_ct = INT64_C(1);
                        }
                    }
                    methods.push_back((*this).function_declaration(std::string("public"), false, method_ct));
                    this->in_class_body = old;
                }
                else {
                    if ((*this).check(TK_IDENTIFIER) && ((*this).peek_at(INT64_C(1)).token_type == TK_LEFT_PAREN)) {
                        auto vname = (*this).consume(TK_IDENTIFIER, std::string("Expect variant name."));
                        (*this).consume(TK_LEFT_PAREN, std::string("Expect '(' after variant name."));
                        std::vector<TypeNode> fields = {};
                        std::vector<std::string> fnames = {};
                        if ((!(*this).check(TK_RIGHT_PAREN))) {
                            fields.push_back((*this).parse_type());
                            fnames.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect field name.")).lexeme);
                            while ((*this).match_any(std::vector{TK_COMMA})) {
                                if ((*this).check(TK_RIGHT_PAREN)) {
                                    break;
                                }
                                fields.push_back((*this).parse_type());
                                fnames.push_back((*this).consume(TK_IDENTIFIER, std::string("Expect field name.")).lexeme);
                            }
                        }
                        (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after variant fields."));
                        variants.push_back(EnumVariantNode(vname, fields, fnames));
                        (*this).match_any(std::vector{TK_NEWLINE});
                    }
                    else {
                        if ((*this).check(TK_IDENTIFIER) && (((*this).peek_at(INT64_C(1)).token_type == TK_NEWLINE) || ((*this).peek_at(INT64_C(1)).token_type == TK_DEDENT))) {
                            auto vname = (*this).advance();
                            std::vector<TypeNode> empty_types = {};
                            std::vector<std::string> empty_fnames = {};
                            variants.push_back(EnumVariantNode(vname, empty_types, empty_fnames));
                            (*this).match_any(std::vector{TK_NEWLINE});
                        }
                        else {
                            TypeNode vtype = (*this).parse_type();
                            auto vname = (*this).consume(TK_IDENTIFIER, std::string("Expect variant name."));
                            std::vector<TypeNode> types = {};
                            std::vector<std::string> old_fnames = {};
                            bool is_unit_type = false;
                            {
                                const auto& _match_141 = vtype;
                                if (std::holds_alternative<std::decay_t<decltype(_match_141)>::NullType>(_match_141._data)) {
                                    is_unit_type = true;
                                }
                                else if (std::holds_alternative<std::decay_t<decltype(_match_141)>::Void>(_match_141._data)) {
                                    is_unit_type = true;
                                }
                                else {
                                    /* pass */
                                }
                            }
                            if ((!is_unit_type)) {
                                types.push_back(vtype);
                                old_fnames.push_back(std::string("value"));
                            }
                            variants.push_back(EnumVariantNode(vname, types, old_fnames));
                            (*this).match_any(std::vector{TK_NEWLINE});
                        }
                    }
                }
            }
        }
        (*this).consume(TK_DEDENT, std::string("Expect dedent to end enum body."));
        return Stmt::make_Enum(name, variants, methods, visibility, type_params);
    }

    Stmt extend_declaration(std::string visibility) {
        if ((!(*this).match_any(std::vector{TK_IDENTIFIER, TK_VECTOR, TK_HASHMAP, TK_HASHSET, TK_STRING_TYPE, TK_BYTES}))) {
            throw std::runtime_error(std::string("Expect type name after 'extend'."));
        }
        auto target = (*this).previous();
        (*this).consume(TK_COLON, std::string("Expect ':' after extend target."));
        (*this).match_any(std::vector{TK_NEWLINE});
        (*this).consume(TK_INDENT, std::string("Expect indentation to start extend body."));
        std::vector<Stmt> methods = {};
        auto old = this->in_class_body;
        this->in_class_body = true;
        while ((!(*this).check(TK_DEDENT)) && (!(*this).is_at_end())) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                /* pass */
            }
            else {
                if ((*this).is_function_start()) {
                    int64_t method_ct = INT64_C(0);
                    if ((*this).match_any(std::vector{TK_COMPTIME_STRICT})) {
                        method_ct = INT64_C(2);
                    }
                    else {
                        if ((*this).match_any(std::vector{TK_COMPTIME})) {
                            method_ct = INT64_C(1);
                        }
                    }
                    methods.push_back((*this).function_declaration(std::string("public"), false, method_ct));
                }
                else {
                    throw std::runtime_error(std::string("Only method declarations allowed in extend block"));
                }
            }
        }
        (*this).consume(TK_DEDENT, std::string("Expect dedent to end extend body."));
        this->in_class_body = old;
        return Stmt::make_Extend(target, methods, visibility);
    }

    Token consume_module_name() {
        if ((*this).match_any(std::vector{TK_IDENTIFIER})) {
            return (*this).previous();
        }
        if ((*this).match_any(std::vector{TK_BYTES})) {
            auto tok = (*this).previous();
            return Token(TK_IDENTIFIER, tok.lexeme, tok.line, tok.col);
        }
        throw std::runtime_error(((((((std::string("Expect module name. Got ") + ((*this).peek().token_type)) + std::string(" at ")) + ((*this).peek().line)) + std::string(":")) + ((*this).peek().col)) + std::string("")));
    }

    Stmt import_statement() {
        std::vector<Token> path = {};
        path.push_back((*this).consume_module_name());
        while ((*this).match_any(std::vector{TK_DOUBLE_COLON})) {
            path.push_back((*this).consume_module_name());
        }
        std::string alias = std::string("");
        if ((*this).match_any(std::vector{TK_AS})) {
            alias = (*this).consume(TK_IDENTIFIER, std::string("Expect alias name after 'as'.")).lexeme;
        }
        (*this).match_any(std::vector{TK_SEMICOLON, TK_NEWLINE});
        return Stmt::make_Import(path, alias);
    }

    Stmt namespace_statement(std::string visibility) {
        auto name = (*this).consume(TK_IDENTIFIER, std::string("Expect namespace name."));
        (*this).consume(TK_COLON, std::string("Expect ':' after namespace name."));
        auto body = (*this).block();
        return Stmt::make_Namespace(name, body, visibility);
    }

    Stmt try_statement() {
        (*this).consume(TK_COLON, std::string("Expect ':' after 'try'."));
        Stmt try_body = Stmt::make_Block((*this).block());
        (*this).consume(TK_CATCH, std::string("Expect 'catch' after 'try' block."));
        std::string exception_name = std::string("");
        if ((*this).match_any(std::vector{TK_IDENTIFIER})) {
            exception_name = (*this).previous().lexeme;
        }
        (*this).consume(TK_COLON, std::string("Expect ':' after 'catch'."));
        Stmt catch_body = Stmt::make_Block((*this).block());
        return Stmt::make_Try(try_body, catch_body, exception_name);
    }

    Stmt declaration() {
        std::string visibility = std::string("public");
        if ((*this).match_any(std::vector{TK_PRIVATE})) {
            visibility = std::string("private");
        }
        else {
            if ((*this).match_any(std::vector{TK_PUBLIC})) {
                visibility = std::string("public");
            }
        }
        bool is_static = (*this).match_any(std::vector{TK_STATIC});
        while ((*this).check(TK_HASH)) {
            while ((!(*this).check(TK_NEWLINE)) && (!(*this).is_at_end())) {
                (*this).advance();
            }
            (*this).match_any(std::vector{TK_NEWLINE});
        }
        if ((*this).match_any(std::vector{TK_EXTERN})) {
            return (*this).extern_declaration();
        }
        if ((*this).match_any(std::vector{TK_IMPORT})) {
            return (*this).import_statement();
        }
        if ((*this).match_any(std::vector{TK_NAMESPACE})) {
            return (*this).namespace_statement(visibility);
        }
        if ((*this).match_any(std::vector{TK_CLASS})) {
            return (*this).class_declaration(visibility);
        }
        if ((*this).match_any(std::vector{TK_STRUCT})) {
            return (*this).struct_declaration(visibility);
        }
        if ((*this).match_any(std::vector{TK_ENUM})) {
            return (*this).enum_declaration(visibility);
        }
        if ((*this).match_any(std::vector{TK_EXTEND})) {
            return (*this).extend_declaration(visibility);
        }
        if ((*this).match_any(std::vector{TK_TRY})) {
            return (*this).try_statement();
        }
        int64_t comptime_mode = INT64_C(0);
        if ((*this).match_any(std::vector{TK_COMPTIME_STRICT})) {
            comptime_mode = INT64_C(2);
        }
        else {
            if ((*this).match_any(std::vector{TK_COMPTIME})) {
                comptime_mode = INT64_C(1);
            }
        }
        if ((*this).match_any(std::vector{TK_CONST})) {
            return (*this).const_declaration(visibility, comptime_mode);
        }
        if (this->in_class_body && (*this).check(TK_IDENTIFIER)) {
            auto ctor_name = (*this).peek().lexeme;
            if (((ctor_name == std::string("constructor")) || (ctor_name == std::string("destructor"))) && ((*this).peek_at(INT64_C(1)).token_type == TK_LEFT_PAREN)) {
                auto name_tok = (*this).advance();
                (*this).consume(TK_LEFT_PAREN, ((std::string("Expect '(' after ") + (ctor_name)) + std::string(".")));
                std::vector<Param> params = (*this).parse_param_list();
                std::vector<Expr> ctor_defaults = this->last_param_defaults;
                (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after parameters."));
                (*this).consume(TK_COLON, std::string("Expect ':' before body."));
                std::vector<Stmt> body = (*this).block();
                std::vector<std::string> empty_tp = {};
                return Stmt::make_Function(name_tok, params, TypeNode::make_Void(), body, false, INT64_C(0), is_static, visibility, empty_tp, ctor_defaults);
            }
        }
        if ((*this).match_any(std::vector{TK_REF_MUT})) {
            return (*this).var_declaration_with_ref(visibility, true, true);
        }
        if ((*this).match_any(std::vector{TK_REF})) {
            return (*this).var_declaration_with_ref(visibility, true, false);
        }
        if ((*this).is_type_at_pos(INT64_C(0))) {
            int64_t next_pos = (*this).skip_type_tokens(INT64_C(0));
            if (((*this).peek_at(next_pos).token_type == TK_QUESTION)) {
                next_pos = (next_pos + INT64_C(1));
            }
            auto next_token = (*this).peek_at(next_pos).token_type;
            if ((next_token == TK_IDENTIFIER) || (next_token == TK_FN) || (next_token == TK_INLINE) || (next_token == TK_COMPTIME) || (next_token == TK_OPERATOR)) {
                if ((*this).is_function_start()) {
                    return (*this).function_declaration(visibility, is_static, comptime_mode);
                }
                if ((comptime_mode > INT64_C(0))) {
                    return (*this).const_declaration(visibility, comptime_mode);
                }
                return (*this).var_declaration(visibility);
            }
        }
        return (*this).statement();
    }

    Stmt extern_declaration() {
        auto header = (*this).consume(TK_STRING, std::string("Expect header string after 'extern'.")).lexeme;
        std::string import_path = std::string("");
        std::string link_lib = std::string("");
        bool parsing_options = true;
        while (parsing_options) {
            if ((*this).match_any(std::vector{TK_IMPORT})) {
                import_path = (*this).consume(TK_STRING, std::string("Expect path string after 'import'.")).lexeme;
            }
            else {
                if ((*this).match_any(std::vector{TK_LINK})) {
                    link_lib = (*this).consume(TK_STRING, std::string("Expect library string after 'link'.")).lexeme;
                }
                else {
                    parsing_options = false;
                }
            }
        }
        (*this).consume(TK_COLON, std::string("Expect ':' after extern header."));
        (*this).match_any(std::vector{TK_NEWLINE});
        (*this).consume(TK_INDENT, std::string("Expect indentation to start extern body."));
        std::vector<ExternType> types = {};
        std::vector<ExternFn> functions = {};
        while ((!(*this).check(TK_DEDENT)) && (!(*this).is_at_end())) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                /* pass */
            }
            else {
                if ((*this).check(TK_IDENTIFIER) && ((*this).peek().lexeme == std::string("type"))) {
                    (*this).advance();
                    auto type_name = (*this).consume(TK_IDENTIFIER, std::string("Expect type name after 'type'."));
                    std::string cpp_name = type_name.lexeme;
                    if ((*this).match_any(std::vector{TK_EQUAL})) {
                        cpp_name = (*this).consume(TK_STRING, std::string("Expect C++ name string.")).lexeme;
                    }
                    types.push_back(ExternType(type_name.lexeme, cpp_name));
                    (*this).match_any(std::vector{TK_NEWLINE});
                }
                else {
                    TypeNode ret_type = (*this).parse_type();
                    (*this).consume(TK_FN, std::string("Expect 'fn' in extern function declaration."));
                    auto fn_name = (*this).consume(TK_IDENTIFIER, std::string("Expect function name."));
                    (*this).consume(TK_LEFT_PAREN, std::string("Expect '(' after extern function name."));
                    std::vector<Param> params = (*this).parse_param_list();
                    (*this).consume(TK_RIGHT_PAREN, std::string("Expect ')' after extern function params."));
                    std::string fn_cpp_name = fn_name.lexeme;
                    if ((*this).match_any(std::vector{TK_EQUAL})) {
                        fn_cpp_name = (*this).consume(TK_STRING, std::string("Expect C++ name string.")).lexeme;
                    }
                    std::vector<Expr> ext_defaults = this->last_param_defaults;
                    functions.push_back(ExternFn(fn_name.lexeme, fn_cpp_name, ret_type, params, ext_defaults));
                    (*this).match_any(std::vector{TK_NEWLINE});
                }
            }
        }
        (*this).consume(TK_DEDENT, std::string("Expect dedent to end extern body."));
        return Stmt::make_Extern(header, import_path, link_lib, types, functions);
    }

    std::vector<Stmt> parse_program() {
        std::vector<Stmt> statements = {};
        while ((!(*this).is_at_end())) {
            if ((*this).match_any(std::vector{TK_NEWLINE})) {
                /* pass */
            }
            else {
                statements.push_back((*this).declaration());
            }
        }
        return statements;
    }

};

bool higher_order_types_equal(const TypeNode& left, const TypeNode& right) {
    {
        const auto& _match_142 = left;
        if (_match_142._tag == "None") {
            {
                const auto& _match_143 = right;
                if (_match_143._tag == "None") {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Int>(_match_142._data)) {
            {
                const auto& _match_144 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_144)>::Int>(_match_144._data)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Float>(_match_142._data)) {
            {
                const auto& _match_145 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_145)>::Float>(_match_145._data)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Str>(_match_142._data)) {
            {
                const auto& _match_146 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_146)>::Str>(_match_146._data)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Bool>(_match_142._data)) {
            {
                const auto& _match_147 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_147)>::Bool>(_match_147._data)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Void>(_match_142._data)) {
            {
                const auto& _match_148 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_148)>::Void>(_match_148._data)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Auto>(_match_142._data)) {
            {
                const auto& _match_149 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_149)>::Auto>(_match_149._data)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::NullType>(_match_142._data)) {
            {
                const auto& _match_150 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_150)>::NullType>(_match_150._data)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Int8>(_match_142._data)) {
            {
                const auto& _match_151 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_151)>::Int8>(_match_151._data)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Int16>(_match_142._data)) {
            {
                const auto& _match_152 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_152)>::Int16>(_match_152._data)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Int32>(_match_142._data)) {
            {
                const auto& _match_153 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_153)>::Int32>(_match_153._data)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Float32>(_match_142._data)) {
            {
                const auto& _match_154 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_154)>::Float32>(_match_154._data)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::USize>(_match_142._data)) {
            {
                const auto& _match_155 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_155)>::USize>(_match_155._data)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::CString>(_match_142._data)) {
            {
                const auto& _match_156 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_156)>::CString>(_match_156._data)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Bytes>(_match_142._data)) {
            {
                const auto& _match_157 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_157)>::Bytes>(_match_157._data)) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Array>(_match_142._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_142)>::Array>(_match_142._data);
            auto& left_inner = *_v.inner;
            {
                const auto& _match_158 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_158)>::Array>(_match_158._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_158)>::Array>(_match_158._data);
                    auto& right_inner = *_v.inner;
                    return higher_order_types_equal(left_inner, right_inner);
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::HashSet>(_match_142._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_142)>::HashSet>(_match_142._data);
            auto& left_inner = *_v.inner;
            {
                const auto& _match_159 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_159)>::HashSet>(_match_159._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_159)>::HashSet>(_match_159._data);
                    auto& right_inner = *_v.inner;
                    return higher_order_types_equal(left_inner, right_inner);
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::HashMap>(_match_142._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_142)>::HashMap>(_match_142._data);
            auto& left_key = *_v.key_type;
            auto& left_value = *_v.value_type;
            {
                const auto& _match_160 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_160)>::HashMap>(_match_160._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_160)>::HashMap>(_match_160._data);
                    auto& right_key = *_v.key_type;
                    auto& right_value = *_v.value_type;
                    return higher_order_types_equal(left_key, right_key) && higher_order_types_equal(left_value, right_value);
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Nullable>(_match_142._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_142)>::Nullable>(_match_142._data);
            auto& left_inner = *_v.inner;
            {
                const auto& _match_161 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_161)>::Nullable>(_match_161._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_161)>::Nullable>(_match_161._data);
                    auto& right_inner = *_v.inner;
                    return higher_order_types_equal(left_inner, right_inner);
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Ptr>(_match_142._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_142)>::Ptr>(_match_142._data);
            auto& left_inner = *_v.inner;
            {
                const auto& _match_162 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_162)>::Ptr>(_match_162._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_162)>::Ptr>(_match_162._data);
                    auto& right_inner = *_v.inner;
                    return higher_order_types_equal(left_inner, right_inner);
                }
                else {
                    return false;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_142)>::Custom>(_match_142._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_142)>::Custom>(_match_142._data);
            auto& left_name = _v.name;
            auto& left_args = _v.type_args;
            {
                const auto& _match_163 = right;
                if (std::holds_alternative<std::decay_t<decltype(_match_163)>::Custom>(_match_163._data)) {
                    auto& _v = std::get<std::decay_t<decltype(_match_163)>::Custom>(_match_163._data);
                    auto& right_name = _v.name;
                    auto& right_args = _v.type_args;
                    if ((left_name != right_name) || (static_cast<int64_t>(left_args.size()) != static_cast<int64_t>(right_args.size()))) {
                        return false;
                    }
                    for (int64_t i = INT64_C(0); i < static_cast<int64_t>(left_args.size()); i++) {
                        if ((!higher_order_types_equal(left_args[i], right_args[i]))) {
                            return false;
                        }
                    }
                    return true;
                }
                else {
                    return false;
                }
            }
        }
    }
    return false;
}

bool higher_order_is_auto_type(const TypeNode& value_type) {
    {
        const auto& _match_164 = value_type;
        if (std::holds_alternative<std::decay_t<decltype(_match_164)>::Auto>(_match_164._data)) {
            return true;
        }
        else {
            return false;
        }
    }
}

TypeNode higher_order_collection_item_type(const TypeNode& value_type) {
    {
        const auto& _match_165 = value_type;
        if (std::holds_alternative<std::decay_t<decltype(_match_165)>::Array>(_match_165._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_165)>::Array>(_match_165._data);
            auto& inner = *_v.inner;
            return inner;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_165)>::HashSet>(_match_165._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_165)>::HashSet>(_match_165._data);
            auto& inner = *_v.inner;
            return inner;
        }
        else {
            return TypeNode::make_Auto();
        }
    }
}

bool higher_order_type_has_auto(const TypeNode& value_type) {
    {
        const auto& _match_166 = value_type;
        if (std::holds_alternative<std::decay_t<decltype(_match_166)>::Auto>(_match_166._data)) {
            return true;
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_166)>::Array>(_match_166._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_166)>::Array>(_match_166._data);
            auto& inner = *_v.inner;
            return higher_order_type_has_auto(inner);
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_166)>::HashSet>(_match_166._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_166)>::HashSet>(_match_166._data);
            auto& inner = *_v.inner;
            return higher_order_type_has_auto(inner);
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_166)>::HashMap>(_match_166._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_166)>::HashMap>(_match_166._data);
            auto& key_type = *_v.key_type;
            auto& item_type = *_v.value_type;
            return higher_order_type_has_auto(key_type) || higher_order_type_has_auto(item_type);
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_166)>::Custom>(_match_166._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_166)>::Custom>(_match_166._data);
            auto& name = _v.name;
            auto& type_args = _v.type_args;
            for (const auto& arg : type_args) {
                if (higher_order_type_has_auto(arg)) {
                    return true;
                }
            }
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_166)>::Nullable>(_match_166._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_166)>::Nullable>(_match_166._data);
            auto& inner = *_v.inner;
            return higher_order_type_has_auto(inner);
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_166)>::Ptr>(_match_166._data)) {
            auto& _v = std::get<std::decay_t<decltype(_match_166)>::Ptr>(_match_166._data);
            auto& inner = *_v.inner;
            return higher_order_type_has_auto(inner);
        }
        else {
            /* pass */
        }
    }
    return false;
}

bool higher_order_closure_accepts_types(const TypeNode& closure_type, const std::vector<TypeNode>& expected_types) {
    if ((!typed_is_closure_type(closure_type))) {
        return false;
    }
    std::vector<TypeNode> param_types = typed_closure_param_types(closure_type);
    if ((static_cast<int64_t>(param_types.size()) != static_cast<int64_t>(expected_types.size()))) {
        return false;
    }
    for (int64_t i = INT64_C(0); i < static_cast<int64_t>(param_types.size()); i++) {
        {
            const auto& _match_167 = param_types[i];
            if (std::holds_alternative<std::decay_t<decltype(_match_167)>::Auto>(_match_167._data)) {
                /* pass */
            }
            else {
                if ((!higher_order_types_equal(param_types[i], expected_types[i]))) {
                    return false;
                }
            }
        }
    }
    return true;
}

TypeNode higher_order_return_type(std::string target_name, const std::vector<TypedExpr>& args, const TypeNode& first_arg_type) {
    if ((target_name == std::string("__lv_col_map")) && (static_cast<int64_t>(args.size()) >= INT64_C(2))) {
        TypeNode item_input = higher_order_collection_item_type(first_arg_type);
        TypeNode callback_type = typed_expr_type(args[INT64_C(1)]);
        std::vector<TypeNode> expected = std::vector{item_input};
        if (typed_is_closure_type(callback_type)) {
            TypeNode item_type = typed_closure_return_type(callback_type);
            if (higher_order_type_has_auto(item_type)) {
                return TypeNode::make_Auto();
            }
            return TypeNode::make_Array(item_type);
        }
        if ((!higher_order_is_auto_type(item_input))) {
            return TypeNode::make_Array(item_input);
        }
        return TypeNode::make_Auto();
    }
    else {
        if ((target_name == std::string("__lv_col_filter")) && (static_cast<int64_t>(args.size()) >= INT64_C(2))) {
            TypeNode item_input = higher_order_collection_item_type(first_arg_type);
            TypeNode callback_type = typed_expr_type(args[INT64_C(1)]);
            std::vector<TypeNode> expected = std::vector{item_input};
            if (typed_is_closure_type(callback_type) && (higher_order_is_auto_type(item_input) || higher_order_closure_accepts_types(callback_type, expected))) {
                return first_arg_type;
            }
            if ((!higher_order_is_auto_type(item_input))) {
                return first_arg_type;
            }
            return TypeNode::make_Auto();
        }
        else {
            if ((target_name == std::string("__lv_col_reduce")) && (static_cast<int64_t>(args.size()) >= INT64_C(3))) {
                TypeNode item_input = higher_order_collection_item_type(first_arg_type);
                TypeNode init_type = typed_expr_type(args[INT64_C(2)]);
                TypeNode callback_type = typed_expr_type(args[INT64_C(1)]);
                std::vector<TypeNode> expected = std::vector{init_type, item_input};
                if (typed_is_closure_type(callback_type) && (higher_order_is_auto_type(item_input) || higher_order_closure_accepts_types(callback_type, expected))) {
                    return init_type;
                }
                {
                    const auto& _match_168 = init_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_168)>::Auto>(_match_168._data)) {
                        /* pass */
                    }
                    else {
                        return init_type;
                    }
                }
                return TypeNode::make_Auto();
            }
            else {
                if ((target_name == std::string("__lv_col_for_each")) && (static_cast<int64_t>(args.size()) >= INT64_C(2))) {
                    TypeNode item_input = higher_order_collection_item_type(first_arg_type);
                    TypeNode callback_type = typed_expr_type(args[INT64_C(1)]);
                    std::vector<TypeNode> expected = std::vector{item_input};
                    if (typed_is_closure_type(callback_type) && (higher_order_is_auto_type(item_input) || higher_order_closure_accepts_types(callback_type, expected))) {
                        return TypeNode::make_Void();
                    }
                    if ((!higher_order_is_auto_type(item_input))) {
                        return TypeNode::make_Void();
                    }
                    return TypeNode::make_Auto();
                }
            }
        }
    }
    return TypeNode::make_Auto();
}

TypeNode direct_call_return_type(Checker& checker, const TypedCallTarget& target, const TypeNode& first_arg_type) {
    {
        const auto& _match_169 = target.kind;
        if (std::holds_alternative<std::decay_t<decltype(_match_169)>::Intrinsic>(_match_169._data)) {
            return intrinsic_type_return_type(target.intrinsic, first_arg_type);
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_169)>::DirectFunction>(_match_169._data)) {
            TypeNode builtin_type = intrinsic_builtin_call_return_type(target.source_name, first_arg_type);
            {
                const auto& _match_170 = builtin_type;
                if (std::holds_alternative<std::decay_t<decltype(_match_170)>::Auto>(_match_170._data)) {
                    /* pass */
                }
                else {
                    return builtin_type;
                }
            }
            builtin_type = intrinsic_builtin_call_return_type(target.lowered_name, first_arg_type);
            {
                const auto& _match_171 = builtin_type;
                if (std::holds_alternative<std::decay_t<decltype(_match_171)>::Auto>(_match_171._data)) {
                    /* pass */
                }
                else {
                    return builtin_type;
                }
            }
            if ((checker.known_funcs.count(target.source_name) > 0)) {
                return checker.known_funcs[target.source_name].return_type;
            }
            if ((checker.known_funcs.count(target.lowered_name) > 0)) {
                return checker.known_funcs[target.lowered_name].return_type;
            }
            return TypeNode::make_Auto();
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_169)>::DirectConstructor>(_match_169._data)) {
            return TypeNode::make_Custom(target.source_name, {});
        }
        else if (std::holds_alternative<std::decay_t<decltype(_match_169)>::ClosureValue>(_match_169._data)) {
            return TypeNode::make_Auto();
        }
        else {
            return TypeNode::make_Auto();
        }
    }
}

TypeNode semantic_call_return_type(Checker& checker, const TypedCallTarget& target, const std::vector<TypedExpr>& args) {
    TypeNode first_arg_type = TypeNode::make_Auto();
    if ((static_cast<int64_t>(args.size()) > INT64_C(0))) {
        first_arg_type = typed_expr_type(args[INT64_C(0)]);
    }
    TypeNode direct_type = direct_call_return_type(checker, target, first_arg_type);
    {
        const auto& _match_172 = direct_type;
        if (std::holds_alternative<std::decay_t<decltype(_match_172)>::Auto>(_match_172._data)) {
            /* pass */
        }
        else {
            return direct_type;
        }
    }
    TypeNode higher_order_type = higher_order_return_type(target.lowered_name, args, first_arg_type);
    {
        const auto& _match_173 = higher_order_type;
        if (std::holds_alternative<std::decay_t<decltype(_match_173)>::Auto>(_match_173._data)) {
            /* pass */
        }
        else {
            return higher_order_type;
        }
    }
    if ((target.lowered_name == std::string("__lv_col_zip")) && (static_cast<int64_t>(args.size()) >= INT64_C(2))) {
        {
            const auto& _match_174 = first_arg_type;
            if (std::holds_alternative<std::decay_t<decltype(_match_174)>::Array>(_match_174._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_174)>::Array>(_match_174._data);
                auto& left_inner = *_v.inner;
                {
                    const auto& _match_175 = typed_expr_type(args[INT64_C(1)]);
                    if (std::holds_alternative<std::decay_t<decltype(_match_175)>::Array>(_match_175._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_175)>::Array>(_match_175._data);
                        auto& right_inner = *_v.inner;
                        if (higher_order_type_has_auto(left_inner) || higher_order_type_has_auto(right_inner)) {
                            return TypeNode::make_Auto();
                        }
                        return TypeNode::make_Array(TypeNode::make_Custom(std::string("std::pair"), std::vector{left_inner, right_inner}));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else {
                /* pass */
            }
        }
    }
    else {
        if ((target.lowered_name == std::string("__lv_col_take")) || (target.lowered_name == std::string("__lv_col_drop"))) {
            return first_arg_type;
        }
        else {
            if ((target.lowered_name == std::string("__lv_col_enumerate")) && (static_cast<int64_t>(args.size()) >= INT64_C(1))) {
                {
                    const auto& _match_176 = first_arg_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_176)>::Array>(_match_176._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_176)>::Array>(_match_176._data);
                        auto& inner = *_v.inner;
                        if (higher_order_type_has_auto(inner)) {
                            return TypeNode::make_Auto();
                        }
                        return TypeNode::make_Array(TypeNode::make_Custom(std::string("std::pair"), std::vector{TypeNode::make_Int(), inner}));
                    }
                    else {
                        /* pass */
                    }
                }
            }
            else {
                if ((target.lowered_name == std::string("__lv_col_range")) || (target.lowered_name == std::string("__lv_col_range_step"))) {
                    return TypeNode::make_Array(TypeNode::make_Int());
                }
                else {
                    if ((target.lowered_name == std::string("__lv_col_set_union")) || (target.lowered_name == std::string("__lv_col_set_intersection")) || (target.lowered_name == std::string("__lv_col_set_difference"))) {
                        return first_arg_type;
                    }
                }
            }
        }
    }
    return TypeNode::make_Auto();
}

struct TypedLowerResult {
    TypedProgram program;
    std::vector<std::string> errors;

};

struct TypedNormalizer {
    Checker checker;
    std::string current_self_name;
    std::string current_struct_name;
    std::vector<std::unordered_map<std::string, TypeNode>> normalized_scopes;
    std::vector<std::vector<Stmt>> raw_lambda_blocks;
    std::vector<std::vector<TypedStmt>> normalized_lambda_blocks;
    std::vector<BodyId> lambda_body_ids;
    int64_t next_item_id;
    int64_t next_body_id;

    TypedNormalizer(const std::vector<Stmt>& stmts, const std::vector<std::vector<Stmt>>& lambda_blocks)
        : raw_lambda_blocks(lambda_blocks) {
        this->checker = Checker();
        this->checker.check(stmts);
        this->current_self_name = std::string("");
        this->current_struct_name = std::string("");
        this->normalized_scopes = {};
        this->normalized_lambda_blocks = {};
        this->lambda_body_ids = {};
        this->next_item_id = INT64_C(0);
        this->next_body_id = INT64_C(0);
    }

    ItemId allocate_item_id() {
        ItemId id = item_id(this->next_item_id);
        this->next_item_id = (this->next_item_id + INT64_C(1));
        return id;
    }

    BodyId allocate_body_id() {
        BodyId id = body_id(this->next_body_id);
        this->next_body_id = (this->next_body_id + INT64_C(1));
        return id;
    }

    void push_scope() {
        std::unordered_map<std::string, TypeNode> scope = {{}};
        this->normalized_scopes.push_back(scope);
    }

    void pop_scope() {
        if ((static_cast<int64_t>(this->normalized_scopes.size()) > INT64_C(0))) {
            lv_pop(this->normalized_scopes);
        }
    }

    void declare_normalized(std::string name, TypeNode value_type) {
        if ((static_cast<int64_t>(this->normalized_scopes.size()) == INT64_C(0))) {
            (*this).push_scope();
        }
        auto& scope = this->normalized_scopes[(static_cast<int64_t>(this->normalized_scopes.size()) - INT64_C(1))];
        scope[name] = value_type;
    }

    TypeNode lookup_normalized(std::string name) {
        int64_t i = (static_cast<int64_t>(this->normalized_scopes.size()) - INT64_C(1));
        while ((i >= INT64_C(0))) {
            auto& scope = this->normalized_scopes[i];
            if ((scope.count(name) > 0)) {
                return scope[name];
            }
            i = (i - INT64_C(1));
        }
        return TypeNode::make_None();
    }

    TypeNode normalized_decl_type(const TypeNode& declared_type, const TypedExpr& value) {
        {
            const auto& _match_177 = declared_type;
            if (std::holds_alternative<std::decay_t<decltype(_match_177)>::Auto>(_match_177._data)) {
                return typed_expr_type(value);
            }
            else if (_match_177._tag == "None") {
                return typed_expr_type(value);
            }
            else {
                return declared_type;
            }
        }
    }

    TypeNode for_item_type(const Expr& collection) {
        TypeNode collection_type = (*this).infer_expr_type(collection);
        {
            const auto& _match_178 = collection_type;
            if (std::holds_alternative<std::decay_t<decltype(_match_178)>::Array>(_match_178._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_178)>::Array>(_match_178._data);
                auto& inner = *_v.inner;
                return inner;
            }
            else {
                /* pass */
            }
        }
        {
            const auto& _match_179 = collection;
            if (std::holds_alternative<std::decay_t<decltype(_match_179)>::Range>(_match_179._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_179)>::Range>(_match_179._data);
                auto& start = *_v.start;
                auto& end = *_v.end;
                return TypeNode::make_Int();
            }
            else {
                return TypeNode::make_Auto();
            }
        }
    }

    bool has_named_args(const std::vector<std::string>& arg_names) {
        for (const auto& name : arg_names) {
            if ((name != std::string(""))) {
                return true;
            }
        }
        return false;
    }

    bool contains_name(const std::vector<std::string>& names, std::string name) {
        for (const auto& existing : names) {
            if ((existing == name)) {
                return true;
            }
        }
        return false;
    }

    std::vector<std::string> copy_names(const std::vector<std::string>& names) {
        std::vector<std::string> copied = {};
        for (const auto& name : names) {
            copied.push_back(name);
        }
        return copied;
    }

    void add_capture(std::vector<std::string>& capture_names, std::vector<TypeNode>& capture_types, std::string name, const TypeNode& value_type) {
        if ((*this).contains_name(capture_names, name)) {
            return;
        }
        capture_names.push_back(name);
        capture_types.push_back(value_type);
    }

    TypeNode field_type_for(std::string struct_name, std::string field_name) {
        if ((this->checker.known_classes.count(struct_name) > 0)) {
            std::vector<Stmt> body = this->checker.known_classes[struct_name];
            for (const auto& member : body) {
                {
                    const auto& _match_180 = member;
                    if (std::holds_alternative<std::decay_t<decltype(_match_180)>::Let>(_match_180._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_180)>::Let>(_match_180._data);
                        auto& name = _v.name;
                        auto& field_type = _v.var_type;
                        auto& initializer = _v.initializer;
                        auto& visibility = _v.visibility;
                        auto& is_ref = _v.is_ref;
                        auto& is_mut = _v.is_mut;
                        if ((name.lexeme == field_name)) {
                            return field_type;
                        }
                    }
                    else {
                        /* pass */
                    }
                }
            }
        }
        return TypeNode::make_Auto();
    }

    ExternFn method_signature_for(std::string struct_name, std::string method_name) {
        std::vector<Param> empty_params = {};
        std::vector<Expr> empty_defaults = {};
        std::string lowered_name = typed_method_name(struct_name, method_name);
        if ((this->checker.known_funcs.count(lowered_name) > 0)) {
            ExternFn full_info = this->checker.known_funcs[lowered_name];
            std::vector<Param> method_params = {};
            if ((static_cast<int64_t>(full_info.params.size()) > INT64_C(0))) {
                int64_t start = INT64_C(0);
                if ((full_info.params[INT64_C(0)].name.lexeme == std::string("self"))) {
                    start = INT64_C(1);
                }
                for (int64_t i = start; i < static_cast<int64_t>(full_info.params.size()); i++) {
                    method_params.push_back(full_info.params[i]);
                }
            }
            std::vector<Expr> method_defaults = {};
            for (const auto& default_expr : full_info.param_defaults) {
                method_defaults.push_back(default_expr);
            }
            return ExternFn(lowered_name, lowered_name, full_info.return_type, method_params, method_defaults);
        }
        if ((this->checker.known_classes.count(struct_name) > 0)) {
            std::vector<Stmt> body = this->checker.known_classes[struct_name];
            for (const auto& member : body) {
                {
                    const auto& _match_181 = member;
                    if (std::holds_alternative<std::decay_t<decltype(_match_181)>::Function>(_match_181._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_181)>::Function>(_match_181._data);
                        auto& name = _v.name;
                        auto& params = _v.params;
                        auto& return_type = _v.return_type;
                        auto& fn_body = _v.body;
                        auto& is_inline = _v.is_inline;
                        auto& comptime_mode = _v.comptime_mode;
                        auto& is_static = _v.is_static;
                        auto& visibility = _v.visibility;
                        auto& type_params = _v.type_params;
                        auto& param_defaults = _v.param_defaults;
                        if ((name.lexeme == method_name)) {
                            return ExternFn(lowered_name, lowered_name, return_type, params, param_defaults);
                        }
                    }
                    else {
                        /* pass */
                    }
                }
            }
        }
        return ExternFn(std::string(""), std::string(""), TypeNode::make_Auto(), empty_params, empty_defaults);
    }

    TypeNode infer_expr_type(const Expr& expr) {
        {
            const auto& _match_182 = expr;
            if (std::holds_alternative<std::decay_t<decltype(_match_182)>::Literal>(_match_182._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_182)>::Literal>(_match_182._data);
                auto& kind = _v.kind;
                auto& value = _v.value;
                if ((kind == std::string("int"))) {
                    return TypeNode::make_Int();
                }
                else {
                    if ((kind == std::string("float"))) {
                        return TypeNode::make_Float();
                    }
                    else {
                        if ((kind == std::string("string"))) {
                            return TypeNode::make_Str();
                        }
                        else {
                            if ((kind == std::string("bool"))) {
                                return TypeNode::make_Bool();
                            }
                            else {
                                if ((kind == std::string("null"))) {
                                    return TypeNode::make_NullType();
                                }
                            }
                        }
                    }
                }
                return TypeNode::make_Auto();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_182)>::Variable>(_match_182._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_182)>::Variable>(_match_182._data);
                auto& name = _v.name;
                if ((this->current_self_name != std::string("")) && (name.lexeme == this->current_self_name) && (this->current_struct_name != std::string(""))) {
                    return TypeNode::make_Custom(this->current_struct_name, {});
                }
                TypeNode normalized_type = (*this).lookup_normalized(name.lexeme);
                {
                    const auto& _match_183 = normalized_type;
                    if (_match_183._tag == "None") {
                        /* pass */
                    }
                    else {
                        return normalized_type;
                    }
                }
                return this->checker.infer_type(expr);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_182)>::Unary>(_match_182._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_182)>::Unary>(_match_182._data);
                auto& op = _v.op;
                auto& right = *_v.right;
                if ((op.token_type == TK_BANG) || (op.token_type == TK_NOT)) {
                    return TypeNode::make_Bool();
                }
                return (*this).infer_expr_type(right);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_182)>::Binary>(_match_182._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_182)>::Binary>(_match_182._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                TypeNode left_type = (*this).infer_expr_type(left);
                TypeNode right_type = (*this).infer_expr_type(right);
                if ((op.token_type == TK_EQUAL_EQUAL) || (op.token_type == TK_BANG_EQUAL) || (op.token_type == TK_LESS) || (op.token_type == TK_LESS_EQUAL) || (op.token_type == TK_GREATER) || (op.token_type == TK_GREATER_EQUAL)) {
                    return TypeNode::make_Bool();
                }
                {
                    const auto& _match_184 = left_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_184)>::Str>(_match_184._data)) {
                        return TypeNode::make_Str();
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_184)>::Float>(_match_184._data)) {
                        return TypeNode::make_Float();
                    }
                    else {
                        /* pass */
                    }
                }
                {
                    const auto& _match_185 = right_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_185)>::Str>(_match_185._data)) {
                        return TypeNode::make_Str();
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_185)>::Float>(_match_185._data)) {
                        return TypeNode::make_Float();
                    }
                    else {
                        /* pass */
                    }
                }
                {
                    const auto& _match_186 = left_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_186)>::Int>(_match_186._data)) {
                        return TypeNode::make_Int();
                    }
                    else {
                        /* pass */
                    }
                }
                return TypeNode::make_Auto();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_182)>::Logical>(_match_182._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_182)>::Logical>(_match_182._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                return TypeNode::make_Bool();
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_182)>::Grouping>(_match_182._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_182)>::Grouping>(_match_182._data);
                auto& inner = *_v.inner;
                return (*this).infer_expr_type(inner);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_182)>::This>(_match_182._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_182)>::This>(_match_182._data);
                auto& keyword = _v.keyword;
                if ((this->current_struct_name != std::string(""))) {
                    return TypeNode::make_Custom(this->current_struct_name, {});
                }
                return this->checker.infer_type(expr);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_182)>::Get>(_match_182._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_182)>::Get>(_match_182._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                TypeNode object_type = (*this).infer_expr_type(object);
                TypeNode pair_type = pair_field_type(object_type, name.lexeme);
                {
                    const auto& _match_187 = pair_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_187)>::Auto>(_match_187._data)) {
                        /* pass */
                    }
                    else {
                        return pair_type;
                    }
                }
                {
                    const auto& _match_188 = object_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_188)>::Custom>(_match_188._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_188)>::Custom>(_match_188._data);
                        auto& struct_name = _v.name;
                        auto& type_args = _v.type_args;
                        return (*this).field_type_for(struct_name, name.lexeme);
                    }
                    else {
                        return TypeNode::make_Auto();
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_182)>::Index>(_match_182._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_182)>::Index>(_match_182._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                TypeNode object_type = (*this).infer_expr_type(object);
                {
                    const auto& _match_189 = object_type;
                    if (std::holds_alternative<std::decay_t<decltype(_match_189)>::Array>(_match_189._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_189)>::Array>(_match_189._data);
                        auto& inner = *_v.inner;
                        return inner;
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_189)>::HashMap>(_match_189._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_189)>::HashMap>(_match_189._data);
                        auto& key_type = *_v.key_type;
                        auto& value_type = *_v.value_type;
                        return value_type;
                    }
                    else {
                        return TypeNode::make_Auto();
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_182)>::Vector>(_match_182._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_182)>::Vector>(_match_182._data);
                auto& elements = _v.elements;
                if ((static_cast<int64_t>(elements.size()) > INT64_C(0))) {
                    return TypeNode::make_Array((*this).infer_expr_type(elements[INT64_C(0)]));
                }
                return TypeNode::make_Array(TypeNode::make_Auto());
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_182)>::Call>(_match_182._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_182)>::Call>(_match_182._data);
                auto& callee = *_v.callee;
                auto& paren = _v.paren;
                auto& args = _v.args;
                auto& arg_names = _v.arg_names;
                {
                    const auto& _match_190 = callee;
                    if (std::holds_alternative<std::decay_t<decltype(_match_190)>::Variable>(_match_190._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_190)>::Variable>(_match_190._data);
                        auto& name = _v.name;
                        TypeNode first_arg_type = TypeNode::make_Auto();
                        if ((static_cast<int64_t>(args.size()) > INT64_C(0))) {
                            first_arg_type = (*this).infer_expr_type(args[INT64_C(0)]);
                        }
                        TypeNode builtin_type = intrinsic_builtin_call_return_type(name.lexeme, first_arg_type);
                        {
                            const auto& _match_191 = builtin_type;
                            if (std::holds_alternative<std::decay_t<decltype(_match_191)>::Auto>(_match_191._data)) {
                                /* pass */
                            }
                            else {
                                return builtin_type;
                            }
                        }
                        if ((this->checker.known_funcs.count(name.lexeme) > 0)) {
                            return this->checker.known_funcs[name.lexeme].return_type;
                        }
                        if ((this->checker.known_classes.count(name.lexeme) > 0)) {
                            return TypeNode::make_Custom(name.lexeme, {});
                        }
                        return this->checker.infer_type(expr);
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_190)>::Get>(_match_190._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_190)>::Get>(_match_190._data);
                        auto& object = *_v.object;
                        auto& name = _v.name;
                        TypeNode object_type = (*this).infer_expr_type(object);
                        TypeNode collection_type = intrinsic_collection_method_return_type(object_type, name.lexeme);
                        {
                            const auto& _match_192 = collection_type;
                            if (std::holds_alternative<std::decay_t<decltype(_match_192)>::Auto>(_match_192._data)) {
                                /* pass */
                            }
                            else {
                                return collection_type;
                            }
                        }
                        {
                            const auto& _match_193 = object_type;
                            if (std::holds_alternative<std::decay_t<decltype(_match_193)>::Custom>(_match_193._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_193)>::Custom>(_match_193._data);
                                auto& struct_name = _v.name;
                                auto& type_args = _v.type_args;
                                return (*this).method_signature_for(struct_name, name.lexeme).return_type;
                            }
                            else {
                                return this->checker.infer_type(expr);
                            }
                        }
                    }
                    else {
                        return this->checker.infer_type(expr);
                    }
                }
            }
            else {
                return this->checker.infer_type(expr);
            }
        }
    }

    TypedCallSig make_typed_call_sig(const std::vector<TypedExpr>& args, const TypeNode& return_type) {
        std::vector<TypeNode> param_types = {};
        for (const auto& arg : args) {
            param_types.push_back(arg.value_type);
        }
        return typed_call_sig(param_types, return_type);
    }

    TypedClosureSig make_typed_closure_sig(const std::vector<TypeNode>& capture_types, const std::vector<TypeNode>& param_types, const TypeNode& return_type) {
        return typed_closure_sig(capture_types, param_types, return_type);
    }

    TypedExpr make_typed_expr(std::string kind, const TypeNode& value_type, const TypedCallTarget& call_target, const TypedCallSig& call_signature, const TypedClosureSig& closure_signature, int64_t int_a, const std::vector<TypedExpr>& children, const std::vector<TypedExpr>& extra_children, const std::vector<std::string>& names, const std::vector<TypeNode>& types, const std::vector<std::string>& capture_names, const std::vector<TypeNode>& capture_types, std::string text_a, std::string text_b) {
        return TypedExpr(kind, value_type, call_target, call_signature, closure_signature, int_a, children, extra_children, names, types, capture_names, capture_types, text_a, text_b);
    }

    std::vector<TypedExpr> normalize_call_args(const ExternFn& fn_info, const std::vector<Expr>& args, const std::vector<std::string>& arg_names) {
        std::vector<TypedExpr> normalized = {};
        bool uses_named = (*this).has_named_args(arg_names);
        if ((static_cast<int64_t>(fn_info.params.size()) == INT64_C(0))) {
            for (const auto& arg : args) {
                normalized.push_back((*this).normalize_expr(arg));
            }
            return normalized;
        }
        if ((!uses_named) && (static_cast<int64_t>(args.size()) == static_cast<int64_t>(fn_info.params.size()))) {
            for (const auto& arg : args) {
                normalized.push_back((*this).normalize_expr(arg));
            }
            return normalized;
        }
        int64_t positional_count = INT64_C(0);
        while ((positional_count < static_cast<int64_t>(arg_names.size())) && (arg_names[positional_count] == std::string(""))) {
            positional_count = (positional_count + INT64_C(1));
        }
        for (int64_t pi = INT64_C(0); pi < static_cast<int64_t>(fn_info.params.size()); pi++) {
            if ((pi < positional_count) && (pi < static_cast<int64_t>(args.size()))) {
                normalized.push_back((*this).normalize_expr(args[pi]));
            }
            else {
                bool found_named = false;
                int64_t ai = positional_count;
                while ((ai < static_cast<int64_t>(arg_names.size()))) {
                    if ((arg_names[ai] == fn_info.params[pi].name.lexeme)) {
                        normalized.push_back((*this).normalize_expr(args[ai]));
                        found_named = true;
                        ai = static_cast<int64_t>(arg_names.size());
                    }
                    else {
                        ai = (ai + INT64_C(1));
                    }
                }
                if ((!found_named)) {
                    if ((pi < static_cast<int64_t>(fn_info.param_defaults.size()))) {
                        {
                            const auto& _match_194 = fn_info.param_defaults[pi];
                            if (_match_194._tag == "None") {
                                normalized.push_back(typed_expr_none());
                            }
                            else {
                                normalized.push_back((*this).normalize_expr(fn_info.param_defaults[pi]));
                            }
                        }
                    }
                    else {
                        normalized.push_back(typed_expr_none());
                    }
                }
            }
        }
        return normalized;
    }

    TypedExpr normalize_expr(const Expr& expr) {
        std::vector<TypedExpr> empty = {};
        std::vector<std::string> empty_names = {};
        std::vector<TypeNode> empty_types = {};
        TypedCallSig no_call = typed_call_sig_none();
        TypedClosureSig no_closure = typed_closure_sig_none();
        {
            const auto& _match_195 = expr;
            if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Literal>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Literal>(_match_195._data);
                auto& kind = _v.kind;
                auto& value = _v.value;
                return (*this).make_typed_expr(std::string("literal"), (*this).infer_expr_type(expr), typed_call_target_none(), no_call, no_closure, INT64_C(0), empty, empty, empty_names, empty_types, empty_names, empty_types, kind, value);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Unary>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Unary>(_match_195._data);
                auto& op = _v.op;
                auto& right = *_v.right;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(right)};
                return (*this).make_typed_expr(std::string("unary"), typed_expr_type(children[INT64_C(0)]), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, op.lexeme, std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Binary>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Binary>(_match_195._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(left), (*this).normalize_expr(right)};
                return (*this).make_typed_expr(std::string("binary"), (*this).infer_expr_type(expr), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, op.lexeme, std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Grouping>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Grouping>(_match_195._data);
                auto& inner = *_v.inner;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(inner)};
                return (*this).make_typed_expr(std::string("grouping"), typed_expr_type(children[INT64_C(0)]), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Variable>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Variable>(_match_195._data);
                auto& name = _v.name;
                return (*this).make_typed_expr(std::string("variable"), (*this).infer_expr_type(expr), typed_call_target_none(), no_call, no_closure, INT64_C(0), empty, empty, empty_names, empty_types, empty_names, empty_types, name.lexeme, std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Assign>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Assign>(_match_195._data);
                auto& name = _v.name;
                auto& value = *_v.value;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(value)};
                return (*this).make_typed_expr(std::string("assign"), typed_expr_type(children[INT64_C(0)]), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, name.lexeme, std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Logical>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Logical>(_match_195._data);
                auto& left = *_v.left;
                auto& op = _v.op;
                auto& right = *_v.right;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(left), (*this).normalize_expr(right)};
                return (*this).make_typed_expr(std::string("logical"), TypeNode::make_Bool(), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, op.lexeme, std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::This>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::This>(_match_195._data);
                auto& keyword = _v.keyword;
                if ((this->current_self_name != std::string(""))) {
                    Expr rewritten = Expr::make_Variable(Token(TK_IDENTIFIER, this->current_self_name, keyword.line, keyword.col));
                    return (*this).make_typed_expr(std::string("variable"), TypeNode::make_Custom(this->current_struct_name, {}), typed_call_target_none(), no_call, no_closure, INT64_C(0), empty, empty, empty_names, empty_types, empty_names, empty_types, this->current_self_name, std::string(""));
                }
                return (*this).make_typed_expr(std::string("this"), (*this).infer_expr_type(expr), typed_call_target_none(), no_call, no_closure, INT64_C(0), empty, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Call>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Call>(_match_195._data);
                auto& callee = *_v.callee;
                auto& paren = _v.paren;
                auto& args = _v.args;
                auto& arg_names = _v.arg_names;
                {
                    const auto& _match_196 = callee;
                    if (std::holds_alternative<std::decay_t<decltype(_match_196)>::Get>(_match_196._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_196)>::Get>(_match_196._data);
                        auto& object = *_v.object;
                        auto& name = _v.name;
                        TypedExpr normalized_object = (*this).normalize_expr(object);
                        TypeNode callee_object_type = typed_expr_type(normalized_object);
                        TypedCallTarget call_target = typed_call_target_from_surface(this->checker, callee, callee_object_type, TypeNode::make_Auto());
                        if (typed_call_is_intrinsic(call_target)) {
                            std::vector<TypedExpr> call_children = std::vector{normalized_object};
                            for (const auto& arg : args) {
                                call_children.push_back((*this).normalize_expr(arg));
                            }
                            TypeNode return_type = semantic_call_return_type(this->checker, call_target, call_children);
                            if ((call_target.lowered_name == std::string("lv_push")) && (static_cast<int64_t>(args.size()) == INT64_C(1))) {
                                TypedExpr normalized_value = (*this).normalize_expr(args[INT64_C(0)]);
                                if ((normalized_object.kind == std::string("get")) && (static_cast<int64_t>(normalized_object.children.size()) > INT64_C(0))) {
                                    std::vector<TypedExpr> pushed_args = std::vector{normalized_object, normalized_value};
                                    TypedCallTarget pushed_target = typed_call_target_intrinsic(IntrinsicId::make_PushCopy());
                                    TypeNode pushed_return_type = semantic_call_return_type(this->checker, pushed_target, pushed_args);
                                    Expr pushed_expr = Expr::make_Call(Expr::make_Variable(Token(TK_IDENTIFIER, pushed_target.lowered_name, name.line, name.col)), paren, {}, {});
                                    TypedExpr pushed_value = (*this).make_typed_expr(std::string("call"), pushed_return_type, pushed_target, (*this).make_typed_call_sig(pushed_args, pushed_return_type), no_closure, INT64_C(0), pushed_args, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
                                    std::vector<TypedExpr> set_children = std::vector{normalized_object.children[INT64_C(0)], pushed_value};
                                    return (*this).make_typed_expr(std::string("set"), pushed_return_type, typed_call_target_none(), no_call, no_closure, INT64_C(0), set_children, empty, empty_names, empty_types, empty_names, empty_types, normalized_object.text_a, std::string(""));
                                }
                                std::vector<TypedExpr> call_children = std::vector{normalized_object, normalized_value};
                                Expr rewritten = Expr::make_Call(Expr::make_Variable(Token(TK_IDENTIFIER, call_target.lowered_name, name.line, name.col)), paren, {}, {});
                                return (*this).make_typed_expr(std::string("call"), return_type, call_target, (*this).make_typed_call_sig(call_children, return_type), no_closure, INT64_C(0), call_children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
                            }
                            Expr rewritten = Expr::make_Call(Expr::make_Variable(Token(TK_IDENTIFIER, call_target.lowered_name, name.line, name.col)), paren, {}, {});
                            return (*this).make_typed_expr(std::string("call"), return_type, call_target, (*this).make_typed_call_sig(call_children, return_type), no_closure, INT64_C(0), call_children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
                        }
                        std::string stdlib_name = stdlib_collection_method_runtime_name(callee_object_type, name.lexeme);
                        if ((stdlib_name != std::string(""))) {
                            std::vector<TypedExpr> call_children = std::vector{normalized_object};
                            for (const auto& arg : args) {
                                call_children.push_back((*this).normalize_expr(arg));
                            }
                            TypedCallTarget stdlib_target = typed_call_target_function(stdlib_name);
                            TypeNode return_type = semantic_call_return_type(this->checker, stdlib_target, call_children);
                            Expr rewritten = Expr::make_Call(Expr::make_Variable(Token(TK_IDENTIFIER, stdlib_target.lowered_name, name.line, name.col)), paren, {}, {});
                            return (*this).make_typed_expr(std::string("call"), return_type, stdlib_target, (*this).make_typed_call_sig(call_children, return_type), no_closure, INT64_C(0), call_children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
                        }
                        {
                            const auto& _match_197 = callee_object_type;
                            if (std::holds_alternative<std::decay_t<decltype(_match_197)>::Custom>(_match_197._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_197)>::Custom>(_match_197._data);
                                auto& struct_name = _v.name;
                                auto& type_args = _v.type_args;
                                ExternFn method_info = (*this).method_signature_for(struct_name, name.lexeme);
                                if ((method_info.name != std::string(""))) {
                                    std::vector<TypedExpr> call_children = std::vector{normalized_object};
                                    std::vector<TypedExpr> method_args = (*this).normalize_call_args(method_info, args, arg_names);
                                    for (const auto& arg : method_args) {
                                        call_children.push_back(arg);
                                    }
                                    Expr rewritten = Expr::make_Call(Expr::make_Variable(Token(TK_IDENTIFIER, (*this).method_signature_for(struct_name, name.lexeme).name, name.line, name.col)), paren, {}, {});
                                    return (*this).make_typed_expr(std::string("call"), method_info.return_type, call_target, (*this).make_typed_call_sig(call_children, method_info.return_type), no_closure, INT64_C(0), call_children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
                                }
                            }
                            else {
                                /* pass */
                            }
                        }
                        std::vector<TypedExpr> fallback_children = std::vector{normalized_object};
                        for (const auto& arg : args) {
                            fallback_children.push_back((*this).normalize_expr(arg));
                        }
                        TypeNode fallback_return = (*this).infer_expr_type(expr);
                        return (*this).make_typed_expr(std::string("call"), fallback_return, call_target, (*this).make_typed_call_sig(fallback_children, fallback_return), no_closure, INT64_C(0), fallback_children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_196)>::Variable>(_match_196._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_196)>::Variable>(_match_196._data);
                        auto& name = _v.name;
                        TypeNode callee_type = (*this).infer_expr_type(callee);
                        TypedCallTarget call_target = typed_call_target_from_surface(this->checker, callee, TypeNode::make_Auto(), callee_type);
                        std::vector<TypedExpr> call_children = {};
                        if ((this->checker.known_funcs.count(name.lexeme) > 0)) {
                            ExternFn fn_info = this->checker.known_funcs[name.lexeme];
                            call_children = (*this).normalize_call_args(fn_info, args, arg_names);
                        }
                        else {
                            for (const auto& arg : args) {
                                call_children.push_back((*this).normalize_expr(arg));
                            }
                        }
                        TypeNode return_type = semantic_call_return_type(this->checker, call_target, call_children);
                        {
                            const auto& _match_198 = return_type;
                            if (std::holds_alternative<std::decay_t<decltype(_match_198)>::Auto>(_match_198._data)) {
                                TypeNode first_arg_type = TypeNode::make_Auto();
                                if ((static_cast<int64_t>(call_children.size()) > INT64_C(0))) {
                                    first_arg_type = typed_expr_type(call_children[INT64_C(0)]);
                                }
                                TypeNode builtin_type = intrinsic_builtin_call_return_type(name.lexeme, first_arg_type);
                                {
                                    const auto& _match_199 = builtin_type;
                                    if (std::holds_alternative<std::decay_t<decltype(_match_199)>::Auto>(_match_199._data)) {
                                        /* pass */
                                    }
                                    else {
                                        return_type = builtin_type;
                                    }
                                }
                            }
                            else {
                                /* pass */
                            }
                        }
                        if (typed_is_closure_type(callee_type)) {
                            TypeNode closure_return = typed_closure_return_type(callee_type);
                            {
                                const auto& _match_200 = closure_return;
                                if (std::holds_alternative<std::decay_t<decltype(_match_200)>::Auto>(_match_200._data)) {
                                    /* pass */
                                }
                                else {
                                    return_type = closure_return;
                                }
                            }
                        }
                        {
                            const auto& _match_201 = return_type;
                            if (std::holds_alternative<std::decay_t<decltype(_match_201)>::Auto>(_match_201._data)) {
                                return_type = (*this).infer_expr_type(expr);
                            }
                            else {
                                /* pass */
                            }
                        }
                        Expr rewritten = Expr::make_Call(Expr::make_Variable(Token(TK_IDENTIFIER, call_target.lowered_name, name.line, name.col)), paren, {}, {});
                        return (*this).make_typed_expr(std::string("call"), return_type, call_target, (*this).make_typed_call_sig(call_children, return_type), no_closure, INT64_C(0), call_children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
                    }
                    else if (std::holds_alternative<std::decay_t<decltype(_match_196)>::StaticGet>(_match_196._data)) {
                        auto& _v = std::get<std::decay_t<decltype(_match_196)>::StaticGet>(_match_196._data);
                        auto& object = *_v.object;
                        auto& name = _v.name;
                        TypeNode callee_object_type = TypeNode::make_Auto();
                        {
                            const auto& _match_202 = object;
                            if (std::holds_alternative<std::decay_t<decltype(_match_202)>::Variable>(_match_202._data)) {
                                auto& _v = std::get<std::decay_t<decltype(_match_202)>::Variable>(_match_202._data);
                                auto& enum_name = _v.name;
                                callee_object_type = TypeNode::make_Custom(enum_name.lexeme, {});
                            }
                            else {
                                /* pass */
                            }
                        }
                        TypedCallTarget call_target = typed_call_target_from_surface(this->checker, callee, callee_object_type, TypeNode::make_Auto());
                        std::vector<TypedExpr> call_children = {};
                        for (const auto& arg : args) {
                            call_children.push_back((*this).normalize_expr(arg));
                        }
                        TypeNode return_type = semantic_call_return_type(this->checker, call_target, call_children);
                        {
                            const auto& _match_203 = return_type;
                            if (std::holds_alternative<std::decay_t<decltype(_match_203)>::Auto>(_match_203._data)) {
                                return_type = (*this).infer_expr_type(expr);
                            }
                            else {
                                /* pass */
                            }
                        }
                        Expr rewritten = Expr::make_Call(Expr::make_Variable(Token(TK_IDENTIFIER, call_target.lowered_name, name.line, name.col)), paren, {}, {});
                        return (*this).make_typed_expr(std::string("call"), return_type, call_target, (*this).make_typed_call_sig(call_children, return_type), no_closure, INT64_C(0), call_children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
                    }
                    else {
                        std::vector<TypedExpr> call_children = {};
                        for (const auto& arg : args) {
                            call_children.push_back((*this).normalize_expr(arg));
                        }
                        TypeNode indirect_return = (*this).infer_expr_type(expr);
                        TypedCallTarget indirect_target = typed_call_target_indirect(std::string(""));
                        return (*this).make_typed_expr(std::string("call"), indirect_return, indirect_target, (*this).make_typed_call_sig(call_children, indirect_return), no_closure, INT64_C(0), call_children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
                    }
                }
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Index>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Index>(_match_195._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(object), (*this).normalize_expr(index)};
                return (*this).make_typed_expr(std::string("index"), (*this).infer_expr_type(expr), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::IndexSet>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::IndexSet>(_match_195._data);
                auto& object = *_v.object;
                auto& bracket = _v.bracket;
                auto& index = *_v.index;
                auto& value = *_v.value;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(object), (*this).normalize_expr(index), (*this).normalize_expr(value)};
                return (*this).make_typed_expr(std::string("index_set"), typed_expr_type(children[INT64_C(2)]), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Vector>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Vector>(_match_195._data);
                auto& elements = _v.elements;
                std::vector<TypedExpr> children = {};
                for (const auto& element : elements) {
                    children.push_back((*this).normalize_expr(element));
                }
                return (*this).make_typed_expr(std::string("vector"), (*this).infer_expr_type(expr), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Map>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Map>(_match_195._data);
                auto& keys = _v.keys;
                auto& values = _v.values;
                std::vector<TypedExpr> key_children = {};
                std::vector<TypedExpr> value_children = {};
                for (const auto& key : keys) {
                    key_children.push_back((*this).normalize_expr(key));
                }
                for (const auto& value : values) {
                    value_children.push_back((*this).normalize_expr(value));
                }
                return (*this).make_typed_expr(std::string("map"), (*this).infer_expr_type(expr), typed_call_target_none(), no_call, no_closure, INT64_C(0), key_children, value_children, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Get>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Get>(_match_195._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(object)};
                return (*this).make_typed_expr(std::string("get"), (*this).infer_expr_type(expr), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, name.lexeme, std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Set>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Set>(_match_195._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                auto& value = *_v.value;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(object), (*this).normalize_expr(value)};
                return (*this).make_typed_expr(std::string("set"), typed_expr_type(children[INT64_C(1)]), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, name.lexeme, std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::StaticGet>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::StaticGet>(_match_195._data);
                auto& object = *_v.object;
                auto& name = _v.name;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(object)};
                return (*this).make_typed_expr(std::string("static_get"), (*this).infer_expr_type(expr), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Lambda>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Lambda>(_match_195._data);
                auto& params = _v.params;
                auto& body = *_v.body;
                (*this).push_scope();
                std::vector<std::string> param_names = {};
                std::vector<TypeNode> param_types = {};
                for (const auto& param : params) {
                    (*this).declare_normalized(param.name.lexeme, param.param_type);
                    param_names.push_back(param.name.lexeme);
                    param_types.push_back(param.param_type);
                }
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(body)};
                std::vector<std::string> capture_names = {};
                std::vector<TypeNode> capture_types = {};
                std::vector<std::string> bound_names = (*this).copy_names(param_names);
                (*this).collect_expr_captures(children[INT64_C(0)], bound_names, capture_names, capture_types);
                (*this).pop_scope();
                TypeNode lambda_return = typed_expr_type(children[INT64_C(0)]);
                TypedClosureSig closure_sig = (*this).make_typed_closure_sig(capture_types, param_types, lambda_return);
                TypeNode closure_type = typed_closure_value_type(closure_sig);
                return (*this).make_typed_expr(std::string("lambda"), closure_type, typed_call_target_none(), no_call, closure_sig, INT64_C(0), children, empty, param_names, param_types, capture_names, capture_types, std::string(""), std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::BlockLambda>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::BlockLambda>(_match_195._data);
                auto& params = _v.params;
                auto& body_id = _v.body_id;
                std::vector<TypedStmt> body = (*this).normalize_lambda_block(params, body_id);
                std::vector<std::string> param_names = {};
                std::vector<TypeNode> param_types = {};
                for (const auto& param : params) {
                    param_names.push_back(param.name.lexeme);
                    param_types.push_back(param.param_type);
                }
                std::vector<std::string> capture_names = {};
                std::vector<TypeNode> capture_types = {};
                std::vector<std::string> bound_names = (*this).copy_names(param_names);
                for (const auto& stmt : body) {
                    (*this).collect_stmt_captures(stmt, bound_names, capture_names, capture_types);
                }
                TypeNode block_return = TypeNode::make_Void();
                for (const auto& stmt : body) {
                    if ((stmt.kind == std::string("return")) && (static_cast<int64_t>(stmt.exprs.size()) > INT64_C(0)) && (stmt.exprs[INT64_C(0)].kind != std::string("none"))) {
                        block_return = stmt.exprs[INT64_C(0)].value_type;
                    }
                }
                TypedClosureSig block_sig = (*this).make_typed_closure_sig(capture_types, param_types, block_return);
                TypeNode closure_type = typed_closure_value_type(block_sig);
                return (*this).make_typed_expr(std::string("block_lambda"), closure_type, typed_call_target_none(), no_call, block_sig, body_id, empty, empty, param_names, param_types, capture_names, capture_types, std::string(""), std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Cast>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Cast>(_match_195._data);
                auto& value = *_v.expr;
                auto& target_type = _v.target_type;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(value)};
                return (*this).make_typed_expr(std::string("cast"), target_type, typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Throw>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Throw>(_match_195._data);
                auto& value = *_v.expr;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(value)};
                return (*this).make_typed_expr(std::string("throw"), TypeNode::make_Void(), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Range>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Range>(_match_195._data);
                auto& start = *_v.start;
                auto& end = *_v.end;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(start), (*this).normalize_expr(end)};
                return (*this).make_typed_expr(std::string("range"), (*this).infer_expr_type(expr), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::Own>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::Own>(_match_195._data);
                auto& value = *_v.expr;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(value)};
                return (*this).make_typed_expr(std::string("own"), typed_expr_type(children[INT64_C(0)]), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_195)>::AddressOf>(_match_195._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_195)>::AddressOf>(_match_195._data);
                auto& value = *_v.expr;
                std::vector<TypedExpr> children = std::vector{(*this).normalize_expr(value)};
                return (*this).make_typed_expr(std::string("address_of"), (*this).infer_expr_type(expr), typed_call_target_none(), no_call, no_closure, INT64_C(0), children, empty, empty_names, empty_types, empty_names, empty_types, std::string(""), std::string(""));
            }
            else {
                return typed_expr_none();
            }
        }
    }

    std::vector<TypedStmt> normalize_block(const std::vector<Stmt>& stmts) {
        std::vector<TypedStmt> normalized = {};
        for (const auto& stmt : stmts) {
            normalized.push_back((*this).normalize_stmt(stmt));
        }
        return normalized;
    }

    void collect_expr_captures(const TypedExpr& expr, std::vector<std::string>& bound_names, std::vector<std::string>& capture_names, std::vector<TypeNode>& capture_types) {
        if ((expr.kind == std::string("variable"))) {
            if ((!(*this).contains_name(bound_names, expr.text_a))) {
                TypeNode capture_type = (*this).lookup_normalized(expr.text_a);
                {
                    const auto& _match_204 = capture_type;
                    if (_match_204._tag == "None") {
                        if ((this->current_self_name != std::string("")) && (expr.text_a == this->current_self_name) && (this->current_struct_name != std::string(""))) {
                            capture_type = TypeNode::make_Custom(this->current_struct_name, {});
                        }
                    }
                    else {
                        /* pass */
                    }
                }
                {
                    const auto& _match_205 = capture_type;
                    if (_match_205._tag == "None") {
                        /* pass */
                    }
                    else {
                        (*this).add_capture(capture_names, capture_types, expr.text_a, capture_type);
                    }
                }
            }
            return;
        }
        else {
            if ((expr.kind == std::string("assign"))) {
                if ((!(*this).contains_name(bound_names, expr.text_a))) {
                    TypeNode capture_type = (*this).lookup_normalized(expr.text_a);
                    {
                        const auto& _match_206 = capture_type;
                        if (_match_206._tag == "None") {
                            /* pass */
                        }
                        else {
                            (*this).add_capture(capture_names, capture_types, expr.text_a, capture_type);
                        }
                    }
                }
            }
            else {
                if ((expr.kind == std::string("lambda")) || (expr.kind == std::string("block_lambda"))) {
                    return;
                }
            }
        }
        for (const auto& child : expr.children) {
            (*this).collect_expr_captures(child, bound_names, capture_names, capture_types);
        }
        for (const auto& child : expr.extra_children) {
            (*this).collect_expr_captures(child, bound_names, capture_names, capture_types);
        }
    }

    void collect_stmt_captures(const TypedStmt& stmt, std::vector<std::string>& bound_names, std::vector<std::string>& capture_names, std::vector<TypeNode>& capture_types) {
        if ((stmt.kind == std::string("let")) || (stmt.kind == std::string("const"))) {
            for (const auto& expr : stmt.exprs) {
                (*this).collect_expr_captures(expr, bound_names, capture_names, capture_types);
            }
            if ((stmt.text_a != std::string(""))) {
                bound_names.push_back(stmt.text_a);
            }
        }
        else {
            if ((stmt.kind == std::string("expr_stmt")) || (stmt.kind == std::string("return"))) {
                for (const auto& expr : stmt.exprs) {
                    (*this).collect_expr_captures(expr, bound_names, capture_names, capture_types);
                }
            }
            else {
                if ((stmt.kind == std::string("block")) || (stmt.kind == std::string("namespace"))) {
                    std::vector<std::string> nested_bound = (*this).copy_names(bound_names);
                    for (const auto& child : stmt.children) {
                        (*this).collect_stmt_captures(child, nested_bound, capture_names, capture_types);
                    }
                }
                else {
                    if ((stmt.kind == std::string("if"))) {
                        for (const auto& expr : stmt.exprs) {
                            (*this).collect_expr_captures(expr, bound_names, capture_names, capture_types);
                        }
                        if ((static_cast<int64_t>(stmt.children.size()) > INT64_C(0))) {
                            std::vector<std::string> then_bound = (*this).copy_names(bound_names);
                            (*this).collect_stmt_captures(stmt.children[INT64_C(0)], then_bound, capture_names, capture_types);
                        }
                        if ((static_cast<int64_t>(stmt.children.size()) > INT64_C(1))) {
                            std::vector<std::string> else_bound = (*this).copy_names(bound_names);
                            (*this).collect_stmt_captures(stmt.children[INT64_C(1)], else_bound, capture_names, capture_types);
                        }
                    }
                    else {
                        if ((stmt.kind == std::string("while"))) {
                            for (const auto& expr : stmt.exprs) {
                                (*this).collect_expr_captures(expr, bound_names, capture_names, capture_types);
                            }
                            if ((static_cast<int64_t>(stmt.children.size()) > INT64_C(0))) {
                                std::vector<std::string> loop_bound = (*this).copy_names(bound_names);
                                (*this).collect_stmt_captures(stmt.children[INT64_C(0)], loop_bound, capture_names, capture_types);
                            }
                        }
                        else {
                            for (const auto& expr : stmt.exprs) {
                                (*this).collect_expr_captures(expr, bound_names, capture_names, capture_types);
                            }
                            for (const auto& child : stmt.children) {
                                (*this).collect_stmt_captures(child, bound_names, capture_names, capture_types);
                            }
                        }
                    }
                }
            }
        }
    }

    std::vector<TypedStmt> normalize_lambda_block(const std::vector<Param>& params, int64_t body_id) {
        while ((static_cast<int64_t>(this->normalized_lambda_blocks.size()) <= body_id)) {
            std::vector<TypedStmt> empty_block = {};
            this->normalized_lambda_blocks.push_back(empty_block);
        }
        while ((static_cast<int64_t>(this->lambda_body_ids.size()) <= body_id)) {
            this->lambda_body_ids.push_back((*this).allocate_body_id());
        }
        if ((static_cast<int64_t>(this->normalized_lambda_blocks[body_id].size()) > INT64_C(0))) {
            return this->normalized_lambda_blocks[body_id];
        }
        (*this).push_scope();
        for (const auto& param : params) {
            (*this).declare_normalized(param.name.lexeme, param.param_type);
        }
        std::vector<TypedStmt> normalized = (*this).normalize_block(this->raw_lambda_blocks[body_id]);
        (*this).pop_scope();
        this->normalized_lambda_blocks[body_id] = normalized;
        return normalized;
    }

    TypedStmt make_typed_stmt(std::string kind, const Stmt& node, const TypeNode& value_type, std::string text_a, std::string text_b, const std::vector<std::string>& names, const std::vector<std::vector<std::string>>& name_groups, const std::vector<TypedExpr>& exprs, const std::vector<TypedStmt>& children) {
        return TypedStmt(kind, node, value_type, text_a, text_b, names, name_groups, exprs, children);
    }

    TypedStmt normalize_stmt(const Stmt& stmt) {
        std::vector<std::string> empty_names = {};
        std::vector<std::vector<std::string>> empty_name_groups = {};
        std::vector<TypedExpr> empty_exprs = {};
        std::vector<TypedStmt> empty_children = {};
        {
            const auto& _match_207 = stmt;
            if (std::holds_alternative<std::decay_t<decltype(_match_207)>::Let>(_match_207._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_207)>::Let>(_match_207._data);
                auto& name = _v.name;
                auto& var_type = _v.var_type;
                auto& initializer = _v.initializer;
                auto& visibility = _v.visibility;
                auto& is_ref = _v.is_ref;
                auto& is_mut = _v.is_mut;
                TypedExpr normalized_initializer = (*this).normalize_expr(initializer);
                TypeNode normalized_type = (*this).normalized_decl_type(var_type, normalized_initializer);
                Stmt lowered = Stmt::make_Let(name, normalized_type, Expr::make_None(), visibility, is_ref, is_mut);
                (*this).declare_normalized(name.lexeme, normalized_type);
                std::vector<TypedExpr> exprs = std::vector{normalized_initializer};
                return (*this).make_typed_stmt(std::string("let"), lowered, normalized_type, name.lexeme, std::string(""), empty_names, empty_name_groups, exprs, empty_children);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_207)>::Const>(_match_207._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_207)>::Const>(_match_207._data);
                auto& name = _v.name;
                auto& const_type = _v.const_type;
                auto& value = _v.value;
                auto& visibility = _v.visibility;
                auto& comptime_mode = _v.comptime_mode;
                TypedExpr normalized_value = (*this).normalize_expr(value);
                TypeNode normalized_type = (*this).normalized_decl_type(const_type, normalized_value);
                Stmt lowered = Stmt::make_Const(name, normalized_type, Expr::make_None(), visibility, comptime_mode);
                (*this).declare_normalized(name.lexeme, normalized_type);
                std::vector<TypedExpr> exprs = std::vector{normalized_value};
                return (*this).make_typed_stmt(std::string("const"), lowered, normalized_type, name.lexeme, std::string(""), empty_names, empty_name_groups, exprs, empty_children);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_207)>::ExprStmt>(_match_207._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_207)>::ExprStmt>(_match_207._data);
                auto& expr = _v.expr;
                std::vector<TypedExpr> exprs = std::vector{(*this).normalize_expr(expr)};
                return (*this).make_typed_stmt(std::string("expr_stmt"), stmt, TypeNode::make_Void(), std::string(""), std::string(""), empty_names, empty_name_groups, exprs, empty_children);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_207)>::Return>(_match_207._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_207)>::Return>(_match_207._data);
                auto& keyword = _v.keyword;
                auto& value = _v.value;
                std::vector<TypedExpr> exprs = std::vector{(*this).normalize_expr(value)};
                return (*this).make_typed_stmt(std::string("return"), stmt, TypeNode::make_Void(), std::string(""), std::string(""), empty_names, empty_name_groups, exprs, empty_children);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_207)>::If>(_match_207._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_207)>::If>(_match_207._data);
                auto& condition = _v.condition;
                auto& then_branch = *_v.then_branch;
                auto& else_branch = *_v.else_branch;
                std::vector<TypedExpr> exprs = std::vector{(*this).normalize_expr(condition)};
                std::vector<TypedStmt> children = std::vector{(*this).normalize_stmt(then_branch), (*this).normalize_stmt(else_branch)};
                return (*this).make_typed_stmt(std::string("if"), stmt, TypeNode::make_Void(), std::string(""), std::string(""), empty_names, empty_name_groups, exprs, children);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_207)>::While>(_match_207._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_207)>::While>(_match_207._data);
                auto& condition = _v.condition;
                auto& body = *_v.body;
                std::vector<TypedExpr> exprs = std::vector{(*this).normalize_expr(condition)};
                std::vector<TypedStmt> children = std::vector{(*this).normalize_stmt(body)};
                return (*this).make_typed_stmt(std::string("while"), stmt, TypeNode::make_Void(), std::string(""), std::string(""), empty_names, empty_name_groups, exprs, children);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_207)>::For>(_match_207._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_207)>::For>(_match_207._data);
                auto& item_name = _v.item_name;
                auto& collection = _v.collection;
                auto& body = *_v.body;
                auto& is_ref = _v.is_ref;
                auto& is_mut = _v.is_mut;
                std::vector<TypedExpr> exprs = std::vector{(*this).normalize_expr(collection)};
                std::vector<TypedStmt> children = std::vector{(*this).normalize_stmt(body)};
                std::string mode = std::string("value");
                if (is_ref && is_mut) {
                    mode = std::string("ref_mut");
                }
                else {
                    if (is_ref) {
                        mode = std::string("ref");
                    }
                }
                (*this).declare_normalized(item_name.lexeme, (*this).for_item_type(collection));
                return (*this).make_typed_stmt(std::string("for"), stmt, TypeNode::make_Void(), item_name.lexeme, mode, empty_names, empty_name_groups, exprs, children);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_207)>::Block>(_match_207._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_207)>::Block>(_match_207._data);
                auto& statements = _v.statements;
                (*this).push_scope();
                std::vector<TypedStmt> children = (*this).normalize_block(statements);
                (*this).pop_scope();
                return (*this).make_typed_stmt(std::string("block"), stmt, TypeNode::make_Void(), std::string(""), std::string(""), empty_names, empty_name_groups, empty_exprs, children);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_207)>::Match>(_match_207._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_207)>::Match>(_match_207._data);
                auto& expr = _v.expr;
                auto& arm_patterns = _v.arm_patterns;
                auto& arm_bodies = _v.arm_bodies;
                std::vector<TypedExpr> exprs = std::vector{(*this).normalize_expr(expr)};
                std::vector<TypedStmt> children = {};
                std::vector<std::string> names = {};
                std::vector<std::vector<std::string>> name_groups = {};
                for (int64_t i = INT64_C(0); i < static_cast<int64_t>(arm_patterns.size()); i++) {
                    names.push_back(arm_patterns[i].pattern_name);
                    name_groups.push_back(arm_patterns[i].bindings);
                    (*this).push_scope();
                    for (const auto& binding : arm_patterns[i].bindings) {
                        (*this).declare_normalized(binding, TypeNode::make_Auto());
                    }
                    children.push_back((*this).normalize_stmt(arm_bodies[i]));
                    (*this).pop_scope();
                }
                return (*this).make_typed_stmt(std::string("match"), stmt, TypeNode::make_Void(), std::string(""), std::string(""), names, name_groups, exprs, children);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_207)>::Try>(_match_207._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_207)>::Try>(_match_207._data);
                auto& try_body = *_v.try_body;
                auto& catch_body = *_v.catch_body;
                auto& exception_name = _v.exception_name;
                std::vector<TypedStmt> children = std::vector{(*this).normalize_stmt(try_body), (*this).normalize_stmt(catch_body)};
                return (*this).make_typed_stmt(std::string("try"), stmt, TypeNode::make_Void(), exception_name, std::string(""), empty_names, empty_name_groups, empty_exprs, children);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_207)>::Namespace>(_match_207._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_207)>::Namespace>(_match_207._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                (*this).push_scope();
                std::vector<TypedStmt> children = (*this).normalize_block(body);
                (*this).pop_scope();
                return (*this).make_typed_stmt(std::string("namespace"), stmt, TypeNode::make_Void(), name.lexeme, std::string(""), empty_names, empty_name_groups, empty_exprs, children);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_207)>::Break>(_match_207._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_207)>::Break>(_match_207._data);
                auto& keyword = _v.keyword;
                return (*this).make_typed_stmt(std::string("break"), stmt, TypeNode::make_Void(), std::string(""), std::string(""), empty_names, empty_name_groups, empty_exprs, empty_children);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_207)>::Continue>(_match_207._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_207)>::Continue>(_match_207._data);
                auto& keyword = _v.keyword;
                return (*this).make_typed_stmt(std::string("continue"), stmt, TypeNode::make_Void(), std::string(""), std::string(""), empty_names, empty_name_groups, empty_exprs, empty_children);
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_207)>::Pass>(_match_207._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_207)>::Pass>(_match_207._data);
                auto& keyword = _v.keyword;
                return (*this).make_typed_stmt(std::string("pass"), stmt, TypeNode::make_Void(), std::string(""), std::string(""), empty_names, empty_name_groups, empty_exprs, empty_children);
            }
            else {
                return typed_stmt_none();
            }
        }
    }

};

TypedLowerResult lower_to_typed(std::string module_name, const std::vector<Stmt>& stmts, const std::vector<std::vector<Stmt>>& lambda_blocks) {
    auto normalizer = TypedNormalizer(stmts, lambda_blocks);
    std::vector<TypedFunctionInfo> functions = {};
    std::vector<TypedStructInfo> structs = {};
    std::vector<TypedEnumInfo> enums = {};
    std::vector<TypedStmt> top_level = {};
    BodyId top_level_body_id = normalizer.allocate_body_id();
    for (const auto& stmt : stmts) {
        {
            const auto& _match_208 = stmt;
            if (std::holds_alternative<std::decay_t<decltype(_match_208)>::Function>(_match_208._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_208)>::Function>(_match_208._data);
                auto& name = _v.name;
                auto& params = _v.params;
                auto& return_type = _v.return_type;
                auto& body = _v.body;
                auto& is_inline = _v.is_inline;
                auto& comptime_mode = _v.comptime_mode;
                auto& is_static = _v.is_static;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                auto& param_defaults = _v.param_defaults;
                normalizer.push_scope();
                for (const auto& param : params) {
                    normalizer.declare_normalized(param.name.lexeme, param.param_type);
                }
                std::vector<TypedStmt> normalized_body = normalizer.normalize_block(body);
                normalizer.pop_scope();
                functions.push_back(TypedFunctionInfo(normalizer.allocate_item_id(), normalizer.allocate_body_id(), name.lexeme, params, return_type, normalized_body));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_208)>::Struct>(_match_208._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_208)>::Struct>(_match_208._data);
                auto& name = _v.name;
                auto& body = _v.body;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                std::vector<TypedStructField> fields = {};
                std::vector<Param> constructor_params = {};
                std::vector<Expr> constructor_defaults = {};
                std::vector<TypedStmt> constructor_body = {};
                ItemId struct_item_id = normalizer.allocate_item_id();
                BodyId constructor_body_id = body_id((-INT64_C(1)));
                std::string saved_self = normalizer.current_self_name;
                std::string saved_struct = normalizer.current_struct_name;
                normalizer.current_self_name = std::string("self");
                normalizer.current_struct_name = name.lexeme;
                normalizer.push_scope();
                normalizer.declare_normalized(std::string("self"), TypeNode::make_Custom(name.lexeme, {}));
                for (const auto& member : body) {
                    {
                        const auto& _match_209 = member;
                        if (std::holds_alternative<std::decay_t<decltype(_match_209)>::Let>(_match_209._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_209)>::Let>(_match_209._data);
                            auto& field_name = _v.name;
                            auto& field_type = _v.var_type;
                            auto& initializer = _v.initializer;
                            auto& field_visibility = _v.visibility;
                            auto& is_ref = _v.is_ref;
                            auto& is_mut = _v.is_mut;
                            fields.push_back(TypedStructField(field_name.lexeme, field_type));
                        }
                        else if (std::holds_alternative<std::decay_t<decltype(_match_209)>::Function>(_match_209._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_209)>::Function>(_match_209._data);
                            auto& fn_name = _v.name;
                            auto& params = _v.params;
                            auto& return_type = _v.return_type;
                            auto& fn_body = _v.body;
                            auto& is_inline = _v.is_inline;
                            auto& comptime_mode = _v.comptime_mode;
                            auto& is_static = _v.is_static;
                            auto& fn_visibility = _v.visibility;
                            auto& fn_type_params = _v.type_params;
                            auto& param_defaults = _v.param_defaults;
                            if ((fn_name.lexeme == std::string("constructor"))) {
                                std::string ctor_self = normalizer.current_self_name;
                                normalizer.push_scope();
                                for (const auto& param : params) {
                                    normalizer.declare_normalized(param.name.lexeme, param.param_type);
                                }
                                normalizer.current_self_name = std::string("");
                                constructor_params = params;
                                constructor_defaults = param_defaults;
                                constructor_body_id = normalizer.allocate_body_id();
                                constructor_body = normalizer.normalize_block(fn_body);
                                normalizer.current_self_name = ctor_self;
                                normalizer.pop_scope();
                            }
                            else {
                                std::vector<Param> lowered_params = std::vector{Param(Token(TK_IDENTIFIER, std::string("self"), name.line, name.col), TypeNode::make_Custom(name.lexeme, {}), true, true)};
                                for (const auto& param : params) {
                                    lowered_params.push_back(param);
                                }
                                normalizer.push_scope();
                                normalizer.declare_normalized(std::string("self"), TypeNode::make_Custom(name.lexeme, {}));
                                for (const auto& param : params) {
                                    normalizer.declare_normalized(param.name.lexeme, param.param_type);
                                }
                                std::vector<TypedStmt> normalized_body = normalizer.normalize_block(fn_body);
                                normalizer.pop_scope();
                                functions.push_back(TypedFunctionInfo(normalizer.allocate_item_id(), normalizer.allocate_body_id(), typed_method_name(name.lexeme, fn_name.lexeme), lowered_params, return_type, normalized_body));
                            }
                        }
                        else {
                            /* pass */
                        }
                    }
                }
                normalizer.pop_scope();
                normalizer.current_self_name = saved_self;
                normalizer.current_struct_name = saved_struct;
                structs.push_back(TypedStructInfo(struct_item_id, constructor_body_id, name.lexeme, fields, constructor_params, constructor_defaults, constructor_body));
            }
            else if (std::holds_alternative<std::decay_t<decltype(_match_208)>::Enum>(_match_208._data)) {
                auto& _v = std::get<std::decay_t<decltype(_match_208)>::Enum>(_match_208._data);
                auto& name = _v.name;
                auto& variants = _v.variants;
                auto& methods = _v.methods;
                auto& visibility = _v.visibility;
                auto& type_params = _v.type_params;
                std::vector<TypedEnumVariant> typed_variants = {};
                ItemId enum_item_id = normalizer.allocate_item_id();
                for (const auto& variant : variants) {
                    typed_variants.push_back(TypedEnumVariant(variant.name.lexeme, variant.types, variant.field_names));
                }
                std::string saved_self = normalizer.current_self_name;
                std::string saved_struct = normalizer.current_struct_name;
                normalizer.current_self_name = std::string("self");
                normalizer.current_struct_name = name.lexeme;
                for (const auto& method : methods) {
                    {
                        const auto& _match_210 = method;
                        if (std::holds_alternative<std::decay_t<decltype(_match_210)>::Function>(_match_210._data)) {
                            auto& _v = std::get<std::decay_t<decltype(_match_210)>::Function>(_match_210._data);
                            auto& fn_name = _v.name;
                            auto& params = _v.params;
                            auto& return_type = _v.return_type;
                            auto& fn_body = _v.body;
                            auto& is_inline = _v.is_inline;
                            auto& comptime_mode = _v.comptime_mode;
                            auto& is_static = _v.is_static;
                            auto& fn_visibility = _v.visibility;
                            auto& fn_type_params = _v.type_params;
                            auto& param_defaults = _v.param_defaults;
                            std::vector<Param> lowered_params = std::vector{Param(Token(TK_IDENTIFIER, std::string("self"), name.line, name.col), TypeNode::make_Custom(name.lexeme, {}), true, true)};
                            for (const auto& param : params) {
                                lowered_params.push_back(param);
                            }
                            normalizer.push_scope();
                            normalizer.declare_normalized(std::string("self"), TypeNode::make_Custom(name.lexeme, {}));
                            for (const auto& param : params) {
                                normalizer.declare_normalized(param.name.lexeme, param.param_type);
                            }
                            std::vector<TypedStmt> normalized_body = normalizer.normalize_block(fn_body);
                            normalizer.pop_scope();
                            functions.push_back(TypedFunctionInfo(normalizer.allocate_item_id(), normalizer.allocate_body_id(), typed_method_name(name.lexeme, fn_name.lexeme), lowered_params, return_type, normalized_body));
                        }
                        else {
                            /* pass */
                        }
                    }
                }
                normalizer.current_self_name = saved_self;
                normalizer.current_struct_name = saved_struct;
                enums.push_back(TypedEnumInfo(enum_item_id, name.lexeme, typed_variants));
            }
            else {
                if ((static_cast<int64_t>(normalizer.normalized_scopes.size()) == INT64_C(0))) {
                    normalizer.push_scope();
                }
                top_level.push_back(normalizer.normalize_stmt(stmt));
            }
        }
    }
    std::vector<TypedLambdaBodyInfo> typed_lambda_blocks = {};
    for (int64_t i = INT64_C(0); i < static_cast<int64_t>(normalizer.normalized_lambda_blocks.size()); i++) {
        typed_lambda_blocks.push_back(TypedLambdaBodyInfo(normalizer.lambda_body_ids[i], normalizer.normalized_lambda_blocks[i]));
    }
    return TypedLowerResult(TypedProgram(module_name, top_level_body_id, top_level, functions, structs, enums, typed_lambda_blocks), normalizer.checker.errors);
}

std::vector<std::string> typed_lower_errors(const TypedLowerResult& result) {
    return result.errors;
}

struct ModuleInfo {
    std::string short_name;
    std::string full_name;
    std::string alias;
    std::string source;

};

struct ImportResolver {
    std::vector<std::string> resolved_paths;
    std::vector<ModuleInfo> modules;
    std::string runtime_dir;
    std::string import_root;

    ImportResolver(std::string runtime_dir, std::string import_root)
        : runtime_dir(runtime_dir), import_root(import_root) {
        this->resolved_paths = {};
        this->modules = {};
    }

    bool already_resolved(const std::string& path) {
        for (const auto& p : this->resolved_paths) {
            if ((p == path)) {
                return true;
            }
        }
        return false;
    }

    std::string get_directory(const std::string& path) {
        int64_t i = (static_cast<int64_t>(path.size()) - INT64_C(1));
        while ((i >= INT64_C(0))) {
            if ((std::string(1, path[i]) == std::string("/"))) {
                return path.substr(INT64_C(0), ((i + INT64_C(1))) - (INT64_C(0)));
            }
            i = (i - INT64_C(1));
        }
        return std::string("./");
    }

    std::string resolve_module_path(const std::string& current_dir, const std::string& file_rel) {
        std::vector<std::string> candidates = {};
        std::string alias_rel = (*this).resolve_compiler_alias(file_rel);
        bool prefer_alias = (alias_rel != std::string("")) && (current_dir == this->import_root);
        if (prefer_alias) {
            candidates.push_back(((((std::string("") + (this->import_root)) + std::string("")) + (alias_rel)) + std::string(".lv")));
        }
        candidates.push_back(((((std::string("") + (current_dir)) + std::string("")) + (file_rel)) + std::string(".lv")));
        if ((alias_rel != std::string("")) && (!prefer_alias)) {
            candidates.push_back(((((std::string("") + (this->import_root)) + std::string("")) + (alias_rel)) + std::string(".lv")));
        }
        if ((this->import_root != current_dir)) {
            candidates.push_back(((((std::string("") + (this->import_root)) + std::string("")) + (file_rel)) + std::string(".lv")));
        }
        for (const auto& candidate : candidates) {
            if (__fs_exists(candidate)) {
                return candidate;
            }
        }
        return candidates[INT64_C(0)];
    }

    std::string resolve_compiler_alias(const std::string& file_rel) {
        if ((file_rel == std::string("backend_cpp"))) {
            return std::string("backend/cpp");
        }
        else {
            if ((file_rel == std::string("backend_cpp_support"))) {
                return std::string("backend/cpp_support");
            }
            else {
                if ((file_rel == std::string("cpp_format"))) {
                    return std::string("backend/cpp_format");
                }
                else {
                    if ((file_rel == std::string("ir_dump"))) {
                        return std::string("ir/dump");
                    }
                    else {
                        if ((file_rel == std::string("ir_call_support"))) {
                            return std::string("ir/support/calls");
                        }
                        else {
                            if ((file_rel == std::string("ir_control_flow_support"))) {
                                return std::string("ir/support/control_flow");
                            }
                            else {
                                if ((file_rel == std::string("ir_metadata_support"))) {
                                    return std::string("ir/support/metadata");
                                }
                                else {
                                    if ((file_rel == std::string("call_semantics"))) {
                                        return std::string("semantics/call_semantics");
                                    }
                                    else {
                                        if ((file_rel == std::string("higher_order"))) {
                                            return std::string("semantics/higher_order");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return std::string("");
    }

    std::string resolve(std::string file_path) {
        if ((*this).already_resolved(file_path)) {
            return std::string("");
        }
        this->resolved_paths.push_back(file_path);
        std::string source = __fs_read(file_path);
        std::string dir = (*this).get_directory(file_path);
        std::string result = std::string("");
        std::vector<std::string> lines = lv_split(source, std::string("\n"));
        for (const auto& line : lines) {
            std::string trimmed = lv_trim(line);
            if (trimmed.starts_with(std::string("import "))) {
                std::string import_str = lv_trim(trimmed.substr(INT64_C(7), (static_cast<int64_t>(trimmed.size())) - (INT64_C(7))));
                std::string alias = std::string("");
                std::string mod_path = import_str;
                int64_t as_pos = lv_index_of(import_str, std::string(" as "));
                if ((as_pos >= INT64_C(0))) {
                    alias = lv_trim(import_str.substr((as_pos + INT64_C(4)), (static_cast<int64_t>(import_str.size())) - ((as_pos + INT64_C(4)))));
                    mod_path = lv_trim(import_str.substr(INT64_C(0), (as_pos) - (INT64_C(0))));
                }
                std::string file_rel = lv_replace(mod_path, std::string("::"), std::string("/"));
                std::vector<std::string> segments = lv_split(file_rel, std::string("/"));
                std::string module_file = (*this).resolve_module_path(dir, file_rel);
                if ((segments[INT64_C(0)] == std::string("std"))) {
                    module_file = ((((std::string("") + (this->runtime_dir)) + std::string("")) + (file_rel)) + std::string(".lv"));
                }
                if ((static_cast<int64_t>(segments.size()) > INT64_C(1))) {
                    std::string short_name = segments[(static_cast<int64_t>(segments.size()) - INT64_C(1))];
                    std::string full_name = lv_join(segments, std::string("_"));
                    std::string mod_source = (*this).resolve(module_file);
                    if ((mod_source != std::string(""))) {
                        this->modules.push_back(ModuleInfo(short_name, full_name, alias, mod_source));
                    }
                }
                else {
                    std::string resolved = (*this).resolve(module_file);
                    if ((resolved != std::string(""))) {
                        result = (result + (resolved + std::string("\n")));
                    }
                }
            }
            else {
                result = (result + (line + std::string("\n")));
            }
        }
        return result;
    }

};

void cleanup(const std::string& cpp_path, const std::string& header_path, const std::string& liblavina_path, bool wrote_header) {
    __os_exec(((std::string("rm -f ") + (cpp_path)) + std::string("")));
    if (wrote_header) {
        __os_exec(((std::string("rm -f ") + (header_path)) + std::string("")));
        __os_exec(((std::string("rm -rf ") + (liblavina_path)) + std::string("")));
    }
}

std::string find_runtime_dir() {
    std::string home = __os_env(std::string("LAVINA_HOME"));
    if ((home != std::string(""))) {
        std::string candidate = ((std::string("") + (home)) + std::string("/runtime"));
        if (__fs_is_dir(candidate)) {
            return ((std::string("") + (candidate)) + std::string("/"));
        }
    }
    std::string exe = __os_exe_path();
    if ((exe != std::string(""))) {
        std::string exe_dir = __fs_dirname(exe);
        std::string candidate = ((std::string("") + (exe_dir)) + std::string("/../lib/lavina/runtime"));
        if (__fs_is_dir(candidate)) {
            return ((std::string("") + (candidate)) + std::string("/"));
        }
    }
    return std::string("runtime/");
}

int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) _lv_args.push_back(argv[i]);
    auto args = __os_args();
    if ((static_cast<int64_t>(args.size()) < INT64_C(2))) {
        print(std::string("Usage: bootstrap [--emit-cpp | --emit-ir | --emit-ir-cpp | compile] <file.lv>"));
        return INT64_C(1);
    }
    std::string mode = std::string("run");
    std::string path = std::string("");
    bool no_check = false;
    int64_t ai = INT64_C(1);
    while ((ai < static_cast<int64_t>(args.size()))) {
        if ((args[ai] == std::string("--no-check"))) {
            no_check = true;
        }
        else {
            if ((args[ai] == std::string("--emit-cpp"))) {
                mode = std::string("emit-cpp");
            }
            else {
                if ((args[ai] == std::string("--emit-ir"))) {
                    mode = std::string("emit-ir");
                }
                else {
                    if ((args[ai] == std::string("--emit-ir-cpp"))) {
                        mode = std::string("emit-ir-cpp");
                    }
                    else {
                        if ((args[ai] == std::string("compile"))) {
                            mode = std::string("compile");
                        }
                        else {
                            path = args[ai];
                        }
                    }
                }
            }
        }
        ai = (ai + INT64_C(1));
    }
    if ((path == std::string(""))) {
        print(std::string("Usage: bootstrap [--emit-cpp | --emit-ir | --emit-ir-cpp | compile] [--no-check] <file.lv>"));
        return INT64_C(1);
    }
    std::string runtime_dir = find_runtime_dir();
    std::string import_root = __fs_dirname(path);
    if ((!import_root.ends_with(std::string("/")))) {
        import_root = (import_root + std::string("/"));
    }
    auto resolver = ImportResolver(runtime_dir, import_root);
    std::string source = resolver.resolve(path);
    std::vector<std::string> mod_short_names = {};
    std::vector<std::string> mod_full_names = {};
    std::vector<std::string> mod_aliases = {};
    std::vector<std::vector<Stmt>> mod_stmts_list = {};
    std::vector<std::vector<Stmt>> all_lambda_blocks = {};
    for (const auto& mod : resolver.modules) {
        auto mod_scanner = Scanner(mod.source);
        mod_scanner.scan_tokens();
        if ((static_cast<int64_t>(mod_scanner.errors.size()) > INT64_C(0))) {
            print(((std::string("Scanner errors in module ") + (mod.full_name)) + std::string(":")));
            for (const auto& err : mod_scanner.errors) {
                print(err);
            }
            return INT64_C(1);
        }
        try {
            auto mod_parser = Parser(mod_scanner.tokens);
            std::vector<Stmt> mod_stmts = mod_parser.parse_program();
            mod_short_names.push_back(mod.short_name);
            mod_full_names.push_back(mod.full_name);
            mod_aliases.push_back(mod.alias);
            mod_stmts_list.push_back(mod_stmts);
            for (const auto& lb : mod_parser.lambda_blocks) {
                all_lambda_blocks.push_back(lb);
            }
        }
         catch (const std::exception& err) {
            print(((((std::string("Error in module ") + (mod.full_name)) + std::string(": ")) + (err.what())) + std::string("")));
            return INT64_C(1);
        }
    }
    auto scanner = Scanner(source);
    scanner.scan_tokens();
    if ((static_cast<int64_t>(scanner.errors.size()) > INT64_C(0))) {
        print(std::string("Scanner errors:"));
        for (const auto& err : scanner.errors) {
            print(err);
        }
        return INT64_C(1);
    }
    std::vector<Stmt> stmts = {};
    std::string cpp = std::string("");
    std::string ir_dump = std::string("");
    std::string ir_cpp = std::string("");
    bool has_main = false;
    std::vector<std::string> link_libs = {};
    std::vector<std::string> import_paths = {};
    try {
        auto parser = Parser(scanner.tokens);
        stmts = parser.parse_program();
        if ((!no_check)) {
            std::vector<Stmt> all_stmts = {};
            for (const auto& ms : mod_stmts_list) {
                for (const auto& s : ms) {
                    all_stmts.push_back(s);
                }
            }
            for (const auto& s : stmts) {
                all_stmts.push_back(s);
            }
            auto check_errors = run_checker(all_stmts);
            if ((static_cast<int64_t>(check_errors.size()) > INT64_C(0))) {
                print(std::string("Type check errors:"));
                for (const auto& cerr : check_errors) {
                    print(((std::string("  ") + (cerr)) + std::string("")));
                }
                return INT64_C(1);
            }
        }
        for (const auto& lb : parser.lambda_blocks) {
            all_lambda_blocks.push_back(lb);
        }
        if ((mode == std::string("emit-ir")) || (mode == std::string("emit-ir-cpp"))) {
            std::vector<Stmt> all_stmts = {};
            for (const auto& ms : mod_stmts_list) {
                for (const auto& s : ms) {
                    all_stmts.push_back(s);
                }
            }
            for (const auto& s : stmts) {
                all_stmts.push_back(s);
            }
            auto typed_result = lower_to_typed(path, all_stmts, all_lambda_blocks);
            auto typed_errors = typed_lower_errors(typed_result);
            if ((static_cast<int64_t>(typed_errors.size()) > INT64_C(0))) {
                print(std::string("Typed lowering errors:"));
                for (const auto& terr : typed_errors) {
                    print(((std::string("  ") + (terr)) + std::string("")));
                }
                return INT64_C(1);
            }
            auto ir_result = lower_to_ir(typed_result.program);
            auto ir_errors = lower_ir_errors(ir_result);
            if ((static_cast<int64_t>(ir_errors.size()) > INT64_C(0))) {
                print(std::string("IR lowering errors:"));
                for (const auto& ierr : ir_errors) {
                    print(((std::string("  ") + (ierr)) + std::string("")));
                }
                return INT64_C(1);
            }
            ir_dump = ir_dump_module(ir_result.module);
            ir_cpp = lower_ir_to_cpp(typed_result.program, ir_result.module);
        }
        auto codegen = CppCodegen();
        codegen.set_modules(mod_short_names, mod_full_names, mod_aliases, mod_stmts_list);
        codegen.lambda_blocks = all_lambda_blocks;
        cpp = codegen.generate(stmts);
        has_main = codegen.has_main;
        link_libs = codegen.extern_link_libs;
        import_paths = codegen.extern_import_paths;
    }
     catch (const std::exception& err) {
        print(((std::string("Error: ") + (err.what())) + std::string("")));
        return INT64_C(1);
    }
    if ((mode == std::string("emit-cpp"))) {
        print(cpp);
        return INT64_C(0);
    }
    if ((mode == std::string("emit-ir"))) {
        print(ir_dump);
        return INT64_C(0);
    }
    if ((mode == std::string("emit-ir-cpp"))) {
        print(ir_cpp);
        return INT64_C(0);
    }
    if ((!has_main)) {
        print(std::string("Error: no main() function defined."));
        return INT64_C(1);
    }
    std::string dir = resolver.get_directory(path);
    std::string base = path;
    int64_t si = (static_cast<int64_t>(path.size()) - INT64_C(1));
    while ((si >= INT64_C(0))) {
        if ((std::string(1, path[si]) == std::string("/"))) {
            base = path.substr((si + INT64_C(1)), (static_cast<int64_t>(path.size())) - ((si + INT64_C(1))));
            break;
        }
        si = (si - INT64_C(1));
    }
    if (base.ends_with(std::string(".lv"))) {
        base = base.substr(INT64_C(0), ((static_cast<int64_t>(base.size()) - INT64_C(3))) - (INT64_C(0)));
    }
    std::string cpp_path = ((((std::string("") + (dir)) + std::string("")) + (base)) + std::string(".cpp"));
    std::string bin_path = ((((std::string("") + (dir)) + std::string("")) + (base)) + std::string(""));
    std::string header_path = ((std::string("") + (dir)) + std::string("lavina.h"));
    __fs_write(cpp_path, cpp);
    bool wrote_header = false;
    std::string liblavina_path = ((std::string("") + (dir)) + std::string("liblavina"));
    if ((!__fs_exists(header_path))) {
        try {
            std::string header_content = __fs_read(((std::string("") + (runtime_dir)) + std::string("lavina.h")));
            __fs_write(header_path, header_content);
            __os_exec(((((std::string("cp -r ") + (runtime_dir)) + std::string("liblavina ")) + (liblavina_path)) + std::string("")));
            wrote_header = true;
        }
         catch (const std::exception& e) {
            print(((std::string("Warning: could not find ") + (runtime_dir)) + std::string("lavina.h")));
        }
    }
    std::string compile_cmd = ((((std::string("g++ -std=c++23 -o ") + (bin_path)) + std::string(" ")) + (cpp_path)) + std::string(""));
    for (const auto& ip : import_paths) {
        compile_cmd = (compile_cmd + ((std::string(" -I") + (ip)) + std::string("")));
    }
    if (__fs_exists(std::string("deps/include"))) {
        bool has_deps = false;
        for (const auto& ip : import_paths) {
            if ((ip == std::string("deps/include"))) {
                has_deps = true;
            }
        }
        if ((!has_deps)) {
            compile_cmd = (compile_cmd + std::string(" -Ideps/include"));
        }
    }
    if (__fs_exists(std::string("deps/lib"))) {
        compile_cmd = (compile_cmd + std::string(" -Ldeps/lib"));
    }
    for (const auto& ll : link_libs) {
        if ((lv_index_of(ll, std::string("/")) >= INT64_C(0))) {
            compile_cmd = (compile_cmd + ((std::string(" ") + (ll)) + std::string("")));
        }
        else {
            compile_cmd = (compile_cmd + ((std::string(" -l") + (ll)) + std::string("")));
        }
    }
    #if defined(_WIN32)
    compile_cmd += " -lws2_32";
    #endif
    int64_t compile_result = __os_exec(compile_cmd);
    if ((compile_result != INT64_C(0))) {
        print(std::string("Compilation failed"));
        cleanup(cpp_path, header_path, liblavina_path, wrote_header);
        return INT64_C(1);
    }
    if ((mode == std::string("compile"))) {
        cleanup(cpp_path, header_path, liblavina_path, wrote_header);
        print(((std::string("Compiled: ") + (bin_path)) + std::string("")));
        return INT64_C(0);
    }
    int64_t run_result = __os_exec(bin_path);
    cleanup(cpp_path, header_path, liblavina_path, wrote_header);
    __os_exec(((std::string("rm -f ") + (bin_path)) + std::string("")));
    if ((run_result != INT64_C(0))) {
        return INT64_C(1);
    }
    return INT64_C(0);
}

