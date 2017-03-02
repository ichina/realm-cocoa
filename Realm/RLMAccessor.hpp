////////////////////////////////////////////////////////////////////////////
//
// Copyright 2017 Realm Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////

#import "RLMAccessor.h"

#import "object_accessor.hpp"

#import "RLMUtil.hpp"

@class RLMRealm;
class RLMClassInfo;
class RLMObservationInfo;

struct RLMOptionalId {
    id value;
    RLMOptionalId(id value) : value(value) { }
    operator id() const noexcept { return value; }
    id operator*() const noexcept { return value; }
};

class RLMAccessorContext {
public:
    RLMAccessorContext(RLMAccessorContext& parent, realm::Property const& property);
    RLMAccessorContext(RLMObjectBase *parentObject, const realm::Property *property = nullptr);
    RLMAccessorContext(RLMRealm *realm, RLMClassInfo& info, bool promote=true);

    id defaultValue(NSString *key);
    id value(id obj, size_t propIndex);

    id box(realm::List&&);
    id box(realm::Results&&);
    id box(realm::Object&&);
    id box(realm::RowExpr);

    id box(realm::BinaryData v) { return RLMBinaryDataToNSData(v); }
    id box(bool v) { return @(v); }
    id box(double v) { return @(v); }
    id box(float v) { return @(v); }
    id box(long long v) { return @(v); }
    id box(realm::StringData v) { return RLMStringDataToNSString(v); }
    id box(realm::Timestamp v) { return RLMTimestampToNSDate(v); }
    id box(realm::Mixed v) { return RLMMixedToObjc(v); }

    id box(realm::util::Optional<bool> v) { return v ? @(*v) : nil; }
    id box(realm::util::Optional<double> v) { return v ? @(*v) : nil; }
    id box(realm::util::Optional<float> v) { return v ? @(*v) : nil; }
    id box(realm::util::Optional<int64_t> v) { return v ? @(*v) : nil; }

    RLMProperty *currentProperty;

    void will_change(realm::Row const&, realm::Property const&);
    void did_change();

    RLMOptionalId value_for_property(id dict, std::string const&, size_t prop_index);
    RLMOptionalId default_value_for_property(realm::Realm*,
                                             realm::ObjectSchema const&,
                                             std::string const& prop);

    template<typename Func>
    void enumerate_list(__unsafe_unretained const id v, Func&& func) {
        for (id value in v) {
            func(value);
        }
    }

    template<typename T>
    T unbox(id v, bool create = false, bool update = false);

    bool is_null(id v) { return v == NSNull.null; }
    id null_value() { return NSNull.null; }
    id no_value() { return nil; }
    bool allow_missing(id v) { return [v isKindOfClass:[NSArray class]]; }

    void will_change(realm::Object& obj, realm::Property const& prop) { will_change(obj.row(), prop); }

    std::string print(id obj) { return [obj description].UTF8String; }

private:
    __unsafe_unretained RLMRealm *const _realm;
    RLMClassInfo& _info;
    bool _promote_existing = true;
    __unsafe_unretained RLMObjectBase *const _parentObject = nil;

    NSDictionary *_defaultValues;

    RLMObservationInfo *_observationInfo = nullptr;
    NSString *_kvoPropertyName = nil;

    id doGetValue(id obj, size_t propIndex, __unsafe_unretained RLMProperty *const prop);
};
