/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

// include required headers
#include "AttributeSettings.h"
#include "Attribute.h"
#include "AttributePool.h"
#include "AttributeFactory.h"
#include "MCoreSystem.h"
#include "StringIdPool.h"
#include "CommandLine.h"
#include "StringConversions.h"


namespace MCore
{
    // default constructor
    AttributeSettings::AttributeSettings()
    {
        mComboValues.SetMemoryCategory(MCORE_MEMCATEGORY_ATTRIBUTES);

        mInterfaceType  = ATTRIBUTE_INTERFACETYPE_DEFAULT;
        mMinValue       = nullptr;
        mMaxValue       = nullptr;
        mDefaultValue   = nullptr;
        mParent         = nullptr;
        mInternalName   = MCORE_INVALIDINDEX32;
        mName           = MCORE_INVALIDINDEX32;
        mFlags          = 0;
    }


    // constructor with internal name
    AttributeSettings::AttributeSettings(const char* internalName)
    {
        mComboValues.SetMemoryCategory(MCORE_MEMCATEGORY_ATTRIBUTES);

        mInterfaceType  = ATTRIBUTE_INTERFACETYPE_DEFAULT;
        mMinValue       = nullptr;
        mMaxValue       = nullptr;
        mDefaultValue   = nullptr;
        mParent         = nullptr;
        mInternalName   = GetStringIdPool().GenerateIdForString(internalName);
        mName           = mInternalName;
        mFlags          = 0;
    }


    // create
    AttributeSettings* AttributeSettings::Create()
    {
        return new AttributeSettings();
    }


    // create
    AttributeSettings* AttributeSettings::Create(const char* internalName)
    {
        return new AttributeSettings(internalName);
    }


    // destructor
    AttributeSettings::~AttributeSettings()
    {
        if (mMinValue)
        {
            mMinValue->Destroy();
        }
        if (mMaxValue)
        {
            mMaxValue->Destroy();
        }
        if (mDefaultValue)
        {
            mDefaultValue->Destroy();
        }
    }


    // delete
    void AttributeSettings::Destroy(bool lock)
    {
        if (lock == false)
        {
            if (mMinValue)
            {
                mMinValue->Destroy(false);
                mMinValue = nullptr;
            }
            if (mMaxValue)
            {
                mMaxValue->Destroy(false);
                mMaxValue = nullptr;
            }
            if (mDefaultValue)
            {
                mDefaultValue->Destroy(false);
                mDefaultValue = nullptr;
            }
        }

        delete this;
    }


    // get the on value changed flag
    bool AttributeSettings::GetReinitGuiOnValueChange() const
    {
        return GetFlag(FLAGINDEX_REINITGUI_ONVALUECHANGE);
    }


    // modify the on value changed flag
    void AttributeSettings::SetReinitGuiOnValueChange(bool enabled)
    {
        SetFlag(FLAGINDEX_REINITGUI_ONVALUECHANGE, enabled);
    }


    // get the on value changed flag
    bool AttributeSettings::GetReinitObjectOnValueChange() const
    {
        return GetFlag(FLAGINDEX_REINITOBJECT_ONVALUECHANGE);
    }


    // modify the on value changed flag
    void AttributeSettings::SetReinitObjectOnValueChange(bool enabled)
    {
        SetFlag(FLAGINDEX_REINITOBJECT_ONVALUECHANGE, enabled);
    }


    //
    uint16 AttributeSettings::GetFlags() const
    {
        return mFlags;
    }


    bool AttributeSettings::GetFlag(uint32 index) const
    {
        return (mFlags & (1 << index)) != 0;
    }


    void AttributeSettings::SetFlag(uint32 index, bool enabled)
    {
        if (enabled)
        {
            mFlags |= (1 << index);
        }
        else
        {
            mFlags &= ~(1 << index);
        }
    }


    void AttributeSettings::SetFlags(uint16 flags)
    {
        mFlags = flags;
    }


    // the equal/copy operator
    AttributeSettings& AttributeSettings::operator=(const AttributeSettings& other)
    {
        // clone the attributes
        if (other.mMinValue)
        {
            mMinValue       = other.mMinValue->Clone();
        }
        if (other.mMaxValue)
        {
            mMaxValue       = other.mMaxValue->Clone();
        }
        if (other.mDefaultValue)
        {
            mDefaultValue   = other.mDefaultValue->Clone();
        }

        // copy the rest
        mName           = other.mName;
        mInternalName   = other.mInternalName;
        mDescription    = other.mDescription;
        mInterfaceType  = other.mInterfaceType;
        mComboValues    = other.mComboValues;
        mFlags          = other.mFlags;
        return *this;
    }


    // set the internal name
    void AttributeSettings::SetInternalName(const char* internalName)
    {
        mInternalName = GetStringIdPool().GenerateIdForString(internalName);
    }


    // set the name
    void AttributeSettings::SetName(const char* name)
    {
        mName = GetStringIdPool().GenerateIdForString(name);
    }


    // set the description
    void AttributeSettings::SetDescription(const char* description)
    {
        mDescription = description;
    }


    // set the interface type
    void AttributeSettings::SetInterfaceType(uint32 interfaceTypeID)
    {
        mInterfaceType = interfaceTypeID;
    }


    // get the internal name
    const char* AttributeSettings::GetInternalName() const
    {
        return (mInternalName != MCORE_INVALIDINDEX32) ? GetStringIdPool().GetName(mInternalName).c_str() : "";
    }


    // get the name
    const char* AttributeSettings::GetName() const
    {
        return (mInternalName != MCORE_INVALIDINDEX32) ? GetStringIdPool().GetName(mName).c_str() : "";
    }


    // get the description
    const char* AttributeSettings::GetDescription() const
    {
        return mDescription.c_str();
    }


    // get the name ID
    uint32 AttributeSettings::GetNameID() const
    {
        return mName;
    }


    // get the internal name ID
    uint32 AttributeSettings::GetInternalNameID() const
    {
        return mInternalName;
    }


    // get a given combobox value string
    const char* AttributeSettings::GetComboValue(uint32 index) const
    {
        return GetStringIdPool().GetName(mComboValues[index]).c_str();
    }


    // get a given combobox value string
    const AZStd::string& AttributeSettings::GetComboValueString(uint32 index) const
    {
        return GetStringIdPool().GetName(mComboValues[index]);
    }


    // get the interface type
    uint32 AttributeSettings::GetInterfaceType() const
    {
        return mInterfaceType;
    }


    // get the internal name as string
    const AZStd::string& AttributeSettings::GetInternalNameString() const
    {
        return GetStringIdPool().GetName(mInternalName);
    }


    // get the name as string
    const AZStd::string& AttributeSettings::GetNameString() const
    {
        return GetStringIdPool().GetName(mName);
    }


    // get a description string
    const AZStd::string& AttributeSettings::GetDescriptionString() const
    {
        return mDescription;
    }


    // get the combobox values
    const Array<uint32>& AttributeSettings::GetComboValues() const
    {
        return mComboValues;
    }


    // get the combobox values with write access
    Array<uint32>& AttributeSettings::GetComboValues()
    {
        return mComboValues;
    }


    // get the number of combobox values
    uint32 AttributeSettings::GetNumComboValues() const
    {
        return mComboValues.GetLength();
    }


    // reserve a given amount of data for the combobox values
    void AttributeSettings::ReserveComboValues(uint32 numToReserve)
    {
        mComboValues.Reserve(numToReserve);
    }


    // resize the combobox array
    void AttributeSettings::ResizeComboValues(uint32 numToResize)
    {
        mComboValues.Resize(numToResize);
    }


    // add a combobox value
    void AttributeSettings::AddComboValue(const char* value)
    {
        mComboValues.Add(GetStringIdPool().GenerateIdForString(value));
    }


    // set a given combobox value
    void AttributeSettings::SetComboValue(uint32 index, const char* value)
    {
        mComboValues[index] = GetStringIdPool().GenerateIdForString(value);
    }


    // get the default value
    Attribute* AttributeSettings::GetDefaultValue() const
    {
        return mDefaultValue;
    }


    // get the minimum value
    Attribute* AttributeSettings::GetMinValue() const
    {
        return mMinValue;
    }


    // get the maximum value
    Attribute* AttributeSettings::GetMaxValue() const
    {
        return mMaxValue;
    }


    // set the default value
    void AttributeSettings::SetDefaultValue(Attribute* value, bool destroyCurrent)
    {
        if (mDefaultValue != value && mDefaultValue && destroyCurrent)
        {
            mDefaultValue->Destroy();
        }

        mDefaultValue = value;
    }


    // set the minimum value
    void AttributeSettings::SetMinValue(Attribute* value, bool destroyCurrent)
    {
        if (mMinValue != value && mMinValue && destroyCurrent)
        {
            mMinValue->Destroy();
        }

        mMinValue = value;
    }


    // set the maximum value
    void AttributeSettings::SetMaxValue(Attribute* value, bool destroyCurrent)
    {
        if (mMaxValue != value && mMaxValue && destroyCurrent)
        {
            mMaxValue->Destroy();
        }

        mMaxValue = value;
    }


    // clone the attribute settings
    AttributeSettings* AttributeSettings::Clone() const
    {
        AttributeSettings* newSettings = new AttributeSettings();
        *newSettings = *this; // use the = operator
        return newSettings;
    }


    // init from another settings object
    void AttributeSettings::InitFrom(const AttributeSettings* other)
    {
        mName           = other->mName;
        mInternalName   = other->mInternalName;
        mInterfaceType  = other->mInterfaceType;
        mComboValues    = other->mComboValues;
        mDescription    = other->mDescription;
        mFlags          = other->mFlags;

        // if the default value is nullptr, try to see if we can clone it
        if (mDefaultValue == nullptr)
        {
            mDefaultValue = (other->mDefaultValue) ? other->mDefaultValue->Clone() : nullptr;
        }
        else
        {
            // the other value isn't nullptr either, so lets see if they are the same type, and if so, copy over the data
            if (other->mDefaultValue)
            {
                if (mDefaultValue->GetType() == other->mDefaultValue->GetType())
                {
                    mDefaultValue->InitFrom(other->mDefaultValue);
                }
                else
                {
                    LogWarning("AttributeSettings::InitFrom() - Trying to init default attribute value '%s' (type=%s) from an attribute '%s' (type=%s) who has another type. The types are incompatible.", GetInternalName(), mDefaultValue->GetTypeString(), other->GetInternalName(), other->mDefaultValue->GetTypeString());
                }
            }
            else
            {
                LogWarning("AttributeSettings::InitFrom() - Trying to init default attribute value '%s' (type=%s) from a nullptr valued attribute (%s).", GetInternalName(), mDefaultValue->GetTypeString(), other->GetInternalName());
            }
        }


        // if the minimum value is nullptr, try to see if we can clone it
        if (mMinValue == nullptr)
        {
            mMinValue = (other->mMinValue) ? other->mMinValue->Clone() : nullptr;
        }
        else
        {
            // the other value isn't nullptr either, so lets see if they are the same type, and if so, copy over the data
            if (other->mMinValue)
            {
                if (mMinValue->GetType() == other->mMinValue->GetType())
                {
                    mMinValue->InitFrom(other->mMinValue);
                }
                else
                {
                    LogWarning("AttributeSettings::InitFrom() - Trying to init minimum attribute value '%s' (type=%s) from an attribute '%s' (type=%s) who has another type. The types are incompatible.", GetInternalName(), mMinValue->GetTypeString(), other->GetInternalName(), other->mMinValue->GetTypeString());
                }
            }
            else
            {
                LogWarning("AttributeSettings::InitFrom() - Trying to init minimum attribute value '%s' (type=%s) from a nullptr valued attribute (%s).", GetInternalName(), mMinValue->GetTypeString(), other->GetInternalName());
            }
        }


        // if the maximum value is nullptr, try to see if we can clone it
        if (mMaxValue == nullptr)
        {
            mMaxValue = (other->mMaxValue) ? other->mMaxValue->Clone() : nullptr;
        }
        else
        {
            // the other value isn't nullptr either, so lets see if they are the same type, and if so, copy over the data
            if (other->mMaxValue)
            {
                if (mMaxValue->GetType() == other->mMaxValue->GetType())
                {
                    mMaxValue->InitFrom(other->mMaxValue);
                }
                else
                {
                    LogWarning("AttributeSettings::InitFrom() - Trying to init maximum attribute value '%s' (type=%s) from an attribute '%s' (type=%s) who has another type. The types are incompatible.", GetInternalName(), mMaxValue->GetTypeString(), other->GetInternalName(), other->mMaxValue->GetTypeString());
                }
            }
            else
            {
                LogWarning("AttributeSettings::InitFrom() - Trying to init maximum attribute value '%s' (type=%s) from a nullptr valued attribute (%s).", GetInternalName(), mMaxValue->GetTypeString(), other->GetInternalName());
            }
        }
    }


    // write to a stream
    bool AttributeSettings::Write(Stream* stream, Endian::EEndianType targetEndianType) const
    {
        // write the version of the attribute settings format
        uint8 version = 2;
        if (stream->Write(&version, sizeof(uint8)) == 0)
        {
            return false;
        }

        uint16 flags = mFlags;
        Endian::ConvertUnsignedInt16To(&flags, targetEndianType);
        if (stream->Write(&flags, sizeof(uint16)) == 0)
        {
            return false;
        }

        // write the attribute internal name string
        uint32 numChars = static_cast<uint32>(GetInternalNameString().size());
        Endian::ConvertUnsignedInt32To(&numChars, targetEndianType);
        if (stream->Write(&numChars, sizeof(uint32)) == 0)
        {
            return false;
        }

        if (numChars > 0)
        {
            if (stream->Write(GetInternalName(), static_cast<uint32>(GetInternalNameString().size())) == 0)
            {
                return false;
            }
        }

        // write the attribute name string
        numChars = static_cast<uint32>(GetNameString().size());
        Endian::ConvertUnsignedInt32To(&numChars, targetEndianType);
        if (stream->Write(&numChars, sizeof(uint32)) == 0)
        {
            return false;
        }

        if (numChars > 0)
        {
            if (stream->Write(GetName(), static_cast<uint32>(GetNameString().size())) == 0)
            {
                return false;
            }
        }

        // write the attribute description
        numChars = static_cast<uint32>(GetDescriptionString().size());
        Endian::ConvertUnsignedInt32To(&numChars, targetEndianType);
        if (stream->Write(&numChars, sizeof(uint32)) == 0)
        {
            return false;
        }

        if (numChars > 0)
        {
            if (stream->Write(GetDescription(), static_cast<uint32>(GetDescriptionString().size())) == 0)
            {
                return false;
            }
        }

        // write interface type
        uint32 interfaceType = mInterfaceType;
        Endian::ConvertUnsignedInt32To(&interfaceType, targetEndianType);
        if (stream->Write(&interfaceType, sizeof(uint32)) == 0)
        {
            return false;
        }

        // combo values
        uint32 numComboValues = GetNumComboValues();
        Endian::ConvertUnsignedInt32To(&numComboValues, targetEndianType);
        if (stream->Write(&numComboValues, sizeof(uint32)) == 0)
        {
            return false;
        }

        // combo strings
        numComboValues = GetNumComboValues();
        for (uint32 i = 0; i < numComboValues; ++i)
        {
            numChars = static_cast<uint32>(GetComboValueString(i).size());
            Endian::ConvertUnsignedInt32To(&numChars, targetEndianType);
            if (stream->Write(&numChars, sizeof(uint32)) == 0)
            {
                return false;
            }

            if (numChars > 0)
            {
                if (stream->Write(GetComboValue(i), static_cast<uint32>(GetComboValueString(i).size())) == 0)
                {
                    return false;
                }
            }
        }

        // save the actual attribute data
        // full attributes means that it saves the type, size, version and its data
        // the default value
        if (Attribute::WriteFullAttribute(stream, targetEndianType, mDefaultValue) == false)
        {
            return false;
        }

        // the minimum value
        if (Attribute::WriteFullAttribute(stream, targetEndianType, mMinValue) == false)
        {
            return false;
        }

        // the maximum value
        if (Attribute::WriteFullAttribute(stream, targetEndianType, mMaxValue) == false)
        {
            return false;
        }

        return true;
    }


    // read a setting
    bool AttributeSettings::Read(Stream* stream, Endian::EEndianType endianType)
    {
        // write the version of the attribute settings format
        uint8 version;
        stream->Read(&version, sizeof(uint8));

        if (version == 1)
        {
            // read the internal name
            uint32 numChars;
            if (stream->Read(&numChars, sizeof(uint32)) == 0)
            {
                return false;
            }
            Endian::ConvertUnsignedInt32(&numChars, endianType);

            AZStd::string tempString;
            tempString.reserve(128);
            tempString.clear(); // keeps the memory
            if (numChars > 0)
            {
                tempString.resize(numChars);
                if (stream->Read(tempString.data(), numChars) == 0)
                {
                    return false;
                }
            }

            SetInternalName(tempString.c_str());

            // read the name
            tempString.clear();
            if (stream->Read(&numChars, sizeof(uint32)) == 0)
            {
                return false;
            }
            Endian::ConvertUnsignedInt32(&numChars, endianType);
            if (numChars > 0)
            {
                tempString.resize(numChars);
                if (stream->Read(tempString.data(), numChars) == 0)
                {
                    return false;
                }
            }
            SetName(tempString.c_str());


            // read the description
            tempString.clear();
            if (stream->Read(&numChars, sizeof(uint32)) == 0)
            {
                return false;
            }
            Endian::ConvertUnsignedInt32(&numChars, endianType);
            if (numChars > 0)
            {
                tempString.resize(numChars);
                if (stream->Read(tempString.data(), numChars) == 0)
                {
                    return false;
                }
            }
            SetDescription(tempString.c_str());


            // read the interface type
            uint32 interfaceType;
            if (stream->Read(&interfaceType, sizeof(uint32)) == 0)
            {
                return false;
            }
            Endian::ConvertUnsignedInt32(&interfaceType, endianType);
            SetInterfaceType(interfaceType);


            // read the number of combobox values
            uint32 numComboValues;
            if (stream->Read(&numComboValues, sizeof(uint32)) == 0)
            {
                return false;
            }
            Endian::ConvertUnsignedInt32(&numComboValues, endianType);
            mComboValues.Resize(numComboValues);

            // read the combo strings
            for (uint32 i = 0; i < numComboValues; ++i)
            {
                tempString.clear();
                if (stream->Read(&numChars, sizeof(uint32)) == 0)
                {
                    return false;
                }
                Endian::ConvertUnsignedInt32(&numChars, endianType);
                if (numChars > 0)
                {
                    tempString.resize(numChars);
                    if (stream->Read(tempString.data(), numChars) == 0)
                    {
                        return false;
                    }
                }
                SetComboValue(i, tempString.c_str());
            }

            // save the actual attribute data
            // full attributes means that it saves the type, size, version and its data
            // the default value
            if (Attribute::ReadFullAttribute(stream, endianType, &mDefaultValue) == false)
            {
                return false;
            }

            // the minimum value
            if (Attribute::ReadFullAttribute(stream, endianType, &mMinValue) == false)
            {
                return false;
            }

            // the maximum value
            if (Attribute::ReadFullAttribute(stream, endianType, &mMaxValue) == false)
            {
                return false;
            }
        }
        else
        {
            MCORE_ASSERT(version == 2);

            // read the flags (new in version 2)
            uint16 flags = 0;
            if (stream->Read(&flags, sizeof(uint16)) == 0)
            {
                return false;
            }
            Endian::ConvertUnsignedInt16(&flags, endianType);
            mFlags = flags;

            // read the internal name
            uint32 numChars;
            if (stream->Read(&numChars, sizeof(uint32)) == 0)
            {
                return false;
            }
            Endian::ConvertUnsignedInt32(&numChars, endianType);

            AZStd::string tempString;
            tempString.reserve(128);
            tempString.clear(); // keeps the memory
            if (numChars > 0)
            {
                tempString.resize(numChars);
                if (stream->Read(tempString.data(), numChars) == 0)
                {
                    return false;
                }
            }

            SetInternalName(tempString.c_str());

            // read the name
            tempString.clear();
            if (stream->Read(&numChars, sizeof(uint32)) == 0)
            {
                return false;
            }
            Endian::ConvertUnsignedInt32(&numChars, endianType);
            if (numChars > 0)
            {
                tempString.resize(numChars);
                if (stream->Read(tempString.data(), numChars) == 0)
                {
                    return false;
                }
            }
            SetName(tempString.c_str());


            // read the description
            tempString.clear();
            if (stream->Read(&numChars, sizeof(uint32)) == 0)
            {
                return false;
            }
            Endian::ConvertUnsignedInt32(&numChars, endianType);
            if (numChars > 0)
            {
                tempString.resize(numChars);
                if (stream->Read(tempString.data(), numChars) == 0)
                {
                    return false;
                }
            }
            SetDescription(tempString.c_str());


            // read the interface type
            uint32 interfaceType;
            if (stream->Read(&interfaceType, sizeof(uint32)) == 0)
            {
                return false;
            }
            Endian::ConvertUnsignedInt32(&interfaceType, endianType);
            SetInterfaceType(interfaceType);


            // read the number of combobox values
            uint32 numComboValues;
            if (stream->Read(&numComboValues, sizeof(uint32)) == 0)
            {
                return false;
            }
            Endian::ConvertUnsignedInt32(&numComboValues, endianType);
            mComboValues.Resize(numComboValues);

            // read the combo strings
            for (uint32 i = 0; i < numComboValues; ++i)
            {
                tempString.clear();
                if (stream->Read(&numChars, sizeof(uint32)) == 0)
                {
                    return false;
                }
                Endian::ConvertUnsignedInt32(&numChars, endianType);
                if (numChars > 0)
                {
                    tempString.resize(numChars);
                    if (stream->Read(tempString.data(), numChars) == 0)
                    {
                        return false;
                    }
                }
                SetComboValue(i, tempString.c_str());
            }

            // save the actual attribute data
            // full attributes means that it saves the type, size, version and its data
            // the default value
            if (Attribute::ReadFullAttribute(stream, endianType, &mDefaultValue) == false)
            {
                return false;
            }

            // the minimum value
            if (Attribute::ReadFullAttribute(stream, endianType, &mMinValue) == false)
            {
                return false;
            }

            // the maximum value
            if (Attribute::ReadFullAttribute(stream, endianType, &mMaxValue) == false)
            {
                return false;
            }
        }

        return true;
    }


    // calculate the size inside the streams
    uint32 AttributeSettings::CalcStreamSize() const
    {
        uint32 totalSize = 0;

        totalSize += sizeof(uint8); // the version
        totalSize += sizeof(uint16); // flags

        totalSize += sizeof(uint32); // numchars for the internal name string
        totalSize += static_cast<uint32>(GetInternalNameString().size());

        totalSize += sizeof(uint32); // numchars for the name string
        totalSize += static_cast<uint32>(GetNameString().size());

        totalSize += sizeof(uint32); // numchars for the description string
        totalSize += static_cast<uint32>(GetDescriptionString().size());

        totalSize += sizeof(uint32); // interface type

        totalSize += sizeof(uint32); // numComboValues
        const uint32 numComboValues = GetNumComboValues();
        for (uint32 i = 0; i < numComboValues; ++i)
        {
            totalSize += sizeof(uint32); // numchars for the combo value string
            totalSize += static_cast<uint32>(GetComboValueString(i).size());
        }

        totalSize += Attribute::GetFullAttributeSize(mDefaultValue);
        totalSize += Attribute::GetFullAttributeSize(mMinValue);
        totalSize += Attribute::GetFullAttributeSize(mMaxValue);

        return totalSize;
    }


    // convert this to a string
    bool AttributeSettings::ConvertToString(AZStd::string& outString) const
    {
        // -name { name } -internalName { intName } -description { descr } -defaultValue { value } -minValue { minValue } -maxValue { maxValue } -flags flags -interfaceType interfaceType -comboValues { value1;value2;value3 }
        outString.clear();
        outString.reserve(1024);

        if (GetNameString().size() > 0)
        {
            outString += "-name";
            outString += " {";
            outString += GetNameString();
            outString += "} ";
        }

        if (GetInternalNameString().size() > 0)
        {
            outString += "-internalName";
            outString += " {";
            outString += GetInternalNameString();
            outString += "} ";
        }

        if (GetDescriptionString().size() > 0)
        {
            outString += "-description";
            outString += " {";
            outString += GetDescriptionString();
            outString += "} ";
        }

        if (mComboValues.GetLength() > 0)
        {
            outString += "-comboValues";
            outString += " {";
            for (uint32 i = 0; i < mComboValues.GetLength(); ++i)
            {
                outString += GetComboValueString(i);
                if (i < mComboValues.GetLength())
                {
                    outString += ";";
                }
            }
            outString += "} ";
        }

        outString += "-interfaceType";
        outString += " ";
        outString += AZStd::to_string((uint32)GetInterfaceType());
        outString += " ";

        outString += "-flags";
        outString += " ";
        outString += AZStd::to_string((uint32)mFlags);
        outString += " ";

        AZStd::string tempString;
        if (mDefaultValue)
        {
            tempString.clear();
            if (mDefaultValue->ConvertToString(tempString) == false)
            {
                LogError("AttributeSettings::ConvertToString() - Failed to convert the default value for settings '%s' to a string.", GetInternalName());
                outString.clear();
                return false;
            }

            outString += "-defaultValue";
            outString += " {";
            outString += "-type ";
            outString += mDefaultValue->GetTypeString();
            outString += " ";
            outString += "-value";
            outString += " {";
            outString += tempString;
            outString += "} ";
            outString += "} ";
        }

        if (mMinValue)
        {
            tempString.clear();
            if (mMinValue->ConvertToString(tempString) == false)
            {
                LogError("AttributeSettings::ConvertToString() - Failed to convert the minimum value for settings '%s' to a string.", GetInternalName());
                outString.clear();
                return false;
            }

            outString += "-minValue";
            outString += " {";
            outString += "-type ";
            outString += mMinValue->GetTypeString();
            outString += " ";
            outString += "-value";
            outString += " {";
            outString += tempString;
            outString += "} ";
            outString += "} ";
        }

        if (mMaxValue)
        {
            tempString.clear();
            if (mMaxValue->ConvertToString(tempString) == false)
            {
                LogError("AttributeSettings::ConvertToString() - Failed to convert the maximum value for settings '%s' to a string.", GetInternalName());
                outString.clear();
                return false;
            }

            outString += "-maxValue";
            outString += " {";
            outString += "-type ";
            outString += mMaxValue->GetTypeString();
            outString += " ";
            outString += "-value";
            outString += " {";
            outString += tempString;
            outString += "} ";
            outString += "} ";
        }

        return true;
    }


    // init from a string
    bool AttributeSettings::InitFromString(const AZStd::string& valueString)
    {
        // -name { name } -internalName { intName } -description { descr } -defaultValue { value } -minValue { minValue } -maxValue { maxValue } -flags flags -interfaceType interfaceType -comboValues { value1;value2;value3 }
        CommandLine commandLine(valueString);

        // set the internal name
        AZStd::string tempString;
        commandLine.GetValue("internalName", "", tempString);
        /*  if (tempString.GetLength() == 0)
            {
                LogError("AttributeSettings::InitFromString() - Failed to extract internal name.");
                return false;
            }*/
        SetInternalName(tempString.c_str());

        // set the name
        commandLine.GetValue("name", "", &tempString);
        if (tempString.size() == 0)
        {
            SetName(GetInternalName());
        }
        else
        {
            SetName(tempString.c_str());
        }

        // set the description
        commandLine.GetValue("description", "", &tempString);
        SetDescription(tempString.c_str());

        // set the interface type
        const uint32 interfaceType = static_cast<uint32>(commandLine.GetValueAsInt("interfaceType", ATTRIBUTE_INTERFACETYPE_DEFAULT));
        if (commandLine.CheckIfHasParameter("interfaceType") == false)
        {
            LogError("AttributeSettings::InitFromString() - Failed to get interface type for attribute '%s'.", GetInternalName());
            return false;
        }
        SetInterfaceType(interfaceType);

        // get the flags
        const uint32 flags = static_cast<uint32>(commandLine.GetValueAsInt("flags", -1));
        if (flags == MCORE_INVALIDINDEX32 || commandLine.CheckIfHasParameter("flags") == false)
        {
            LogError("AttributeSettings::InitFromString() - Failed to get flags for attribute '%s'.", GetInternalName());
            return false;
        }
        mFlags = static_cast<uint16>(flags);

        // set the combo values
        commandLine.GetValue("comboValues", "", &tempString);
        if (tempString.size() > 0)
        {
            AZStd::vector<AZStd::string> comboValues;
            AzFramework::StringFunc::Tokenize(tempString.c_str(), comboValues, MCore::CharacterConstants::semiColon, true /* keep empty strings */, true /* keep space strings */);

            mComboValues.Resize(static_cast<uint32>(comboValues.size()));
            const uint32 comboValuesSize = static_cast<uint32>(comboValues.size());
            for (uint32 i = 0; i < comboValuesSize; ++i)
            {
                SetComboValue(i, comboValues[i].c_str());
            }
        }

        // get the default value string
        AZStd::string typeString;
        commandLine.GetValue("defaultValue", "", &tempString);
        if (tempString.size() > 0)
        {
            // check the type
            CommandLine defaultCommandLine(tempString);
            defaultCommandLine.GetValue("type", "", &typeString);
            if (typeString.size() == 0)
            {
                MCore::LogError("AttributeSettings::InitFromString() - Failed to get the default value type for attribute '%s'.", GetInternalName());
                return false;
            }

            // get the value
            defaultCommandLine.GetValue("value", "", &tempString);
            if (tempString.size() == 0 && defaultCommandLine.CheckIfHasParameter("value") == false)
            {
                MCore::LogError("AttributeSettings::InitFromString() - Failed to get the default value string for attribute '%s' of type '%s'.", GetInternalName(), typeString.c_str());
                return false;
            }

            // create the attribute
            Attribute* value = GetAttributeFactory().CreateAttributeByTypeString(typeString.c_str());
            if (value == nullptr)
            {
                MCore::LogError("AttributeSettings::InitFromString() - Failed to create the default value attribute object for attribute '%s' of type '%s'.", GetInternalName(), typeString.c_str());
                return false;
            }

            // init the attribute from the given data string
            if (value->InitFromString(tempString) == false)
            {
                MCore::LogError("AttributeSettings::InitFromString() - Failed to init the default value attribute object for attribute '%s' of type '%s' from a given string.", GetInternalName(), typeString.c_str());
                value->Destroy();
                return false;
            }

            // set the default value
            SetDefaultValue(value);
        }

        // set the minimum
        commandLine.GetValue("minValue", "", &tempString);
        if (tempString.size() > 0)
        {
            // check the type
            CommandLine minCommandLine(tempString);
            minCommandLine.GetValue("type", "", &typeString);
            if (typeString.size() == 0)
            {
                MCore::LogError("AttributeSettings::InitFromString() - Failed to get the minimum value type for attribute '%s'.", GetInternalName());
                SetDefaultValue(nullptr);
                return false;
            }

            // get the value
            minCommandLine.GetValue("value", "", &tempString);
            if (tempString.size() == 0 && minCommandLine.CheckIfHasParameter("value") == false)
            {
                MCore::LogError("AttributeSettings::InitFromString() - Failed to get the minimum value string for attribute '%s' of type '%s'.", GetInternalName(), typeString.c_str());
                SetDefaultValue(nullptr);
                return false;
            }

            // create the attribute
            Attribute* value = GetAttributeFactory().CreateAttributeByTypeString(typeString.c_str());
            if (value == nullptr)
            {
                MCore::LogError("AttributeSettings::InitFromString() - Failed to create the minimum value attribute object for attribute '%s' of type '%s'.", GetInternalName(), typeString.c_str());
                SetDefaultValue(nullptr);
                return false;
            }

            // init the attribute from the given data string
            if (value->InitFromString(tempString) == false)
            {
                MCore::LogError("AttributeSettings::InitFromString() - Failed to init the minimum value attribute object for attribute '%s' of type '%s' from a given string.", GetInternalName(), typeString.c_str());
                SetDefaultValue(nullptr);
                value->Destroy();
                return false;
            }

            // set the value
            SetMinValue(value);
        }
        else
        {
            SetMinValue(nullptr);
        }


        // set the maximum
        commandLine.GetValue("maxValue", "", &tempString);
        if (tempString.size() > 0)
        {
            // check the type
            CommandLine maxCommandLine(tempString);
            maxCommandLine.GetValue("type", "", &typeString);
            if (typeString.size() == 0)
            {
                MCore::LogError("AttributeSettings::InitFromString() - Failed to get the maximum value type for attribute '%s'.", GetInternalName());
                SetDefaultValue(nullptr);
                SetMinValue(nullptr);
                return false;
            }

            // get the value
            maxCommandLine.GetValue("value", "", &tempString);
            if (tempString.size() == 0 && maxCommandLine.CheckIfHasParameter("value") == false)
            {
                MCore::LogError("AttributeSettings::InitFromString() - Failed to get the maximum value string for attribute '%s' of type '%s'.", GetInternalName(), typeString.c_str());
                SetDefaultValue(nullptr);
                SetMinValue(nullptr);
                return false;
            }

            // create the attribute
            Attribute* value = GetAttributeFactory().CreateAttributeByTypeString(typeString.c_str());
            if (value == nullptr)
            {
                MCore::LogError("AttributeSettings::InitFromString() - Failed to create the maximum value attribute object for attribute '%s' of type '%s'.", GetInternalName(), typeString.c_str());
                SetDefaultValue(nullptr);
                SetMinValue(nullptr);
                return false;
            }

            // init the attribute from the given data string
            if (value->InitFromString(tempString) == false)
            {
                MCore::LogError("AttributeSettings::InitFromString() - Failed to init the maximum value attribute object for attribute '%s' of type '%s' from a given string.", GetInternalName(), typeString.c_str());
                SetDefaultValue(nullptr);
                SetMinValue(nullptr);
                value->Destroy();
                return false;
            }

            // set the value
            SetMaxValue(value);
        }
        else
        {
            SetMaxValue(nullptr);
        }

        return true;
    }


    // get the referencing status
    bool AttributeSettings::GetReferencesOtherAttribute() const
    {
        return GetFlag(FLAGINDEX_REFERENCE_OTHERATTRIBUTE);
    }


    // set if we reference or not
    void AttributeSettings::SetReferencesOtherAttribute(bool doesReference)
    {
        SetFlag(FLAGINDEX_REFERENCE_OTHERATTRIBUTE, doesReference);
    }


    // build a tooltip string
    void AttributeSettings::BuildToolTipString(AZStd::string& outString, MCore::Attribute* value)
    {
        AZStd::string tempString;
        outString.reserve(2048);

        outString = "<table border=\"0\">";
        outString += AZStd::string::format("<tr><td width=\"150\"><b>%s</b></td><td width=\"300\">%s</td></tr>",  "Name: ",           GetName());
        outString += AZStd::string::format("<tr><td><b>%s</b></td><td>%s</td></tr>",              "Internal Name: ",  GetInternalName());

        if (value)
        {
            value->BuildHierarchicalName(tempString);
            outString += AZStd::string::format("<tr><td><b>%s</b></td><td>%s</td></tr>",          "Hierarchical Name: ",  tempString.c_str());
        }

        if (mDescription.size() > 0)
        {
            outString += AZStd::string::format("<tr><td><b>%s</b></td><td>%s</td></tr>",          "Description: ",    GetDescription());
        }

        if (mDefaultValue)
        {
            outString += AZStd::string::format("<tr><td><b>%s</b></td><td>%s</td></tr>",          "Attribute Type: ", mDefaultValue->GetTypeString());

            if (mDefaultValue->ConvertToString(tempString))
            {
                if (tempString.size() > 100)
                {
                    tempString.resize(100);
                }
                outString += AZStd::string::format("<tr><td><b>%s</b></td><td>%s</td></tr>",      "Default Value: ",  tempString.c_str());
            }
        }

        if (mMinValue)
        {
            if (mMinValue->ConvertToString(tempString))
            {
                if (tempString.size() > 100)
                {
                    tempString.resize(100);
                }
                outString += AZStd::string::format("<tr><td><b>%s</b></td><td>%s</td></tr>",      "Minimum Value: ",  tempString.c_str());
            }
        }

        if (mMaxValue)
        {
            if (mMaxValue->ConvertToString(tempString))
            {
                if (tempString.size() > 100)
                {
                    tempString.resize(100);
                }
                outString += AZStd::string::format("<tr><td><b>%s</b></td><td>%s</td></tr>",      "Maximum Value: ",  tempString.c_str());
            }
        }

        outString += AZStd::string::format("<tr><td><b>%s</b></td><td>%d</td></tr>",              "Interface Type ID: ",  mInterfaceType);
        outString += AZStd::string::format("<tr><td><b>%s</b></td><td>%s</td></tr>",              "References Other: ",   GetReferencesOtherAttribute() ? "Yes" : "No");

        outString += "</table>";
    }


    // get the parent
    Attribute* AttributeSettings::GetParent() const
    {
        return mParent;
    }


    // do we have a parent?
    bool AttributeSettings::GetHasParent() const
    {
        return (mParent != nullptr);
    }


    // set the parent pointer
    void AttributeSettings::SetParent(Attribute* parent)
    {
        mParent = parent;
    }


    // find the parent settings
    AttributeSettings* AttributeSettings::FindParentSettings() const
    {
        if (mParent)
        {
            Attribute* parentOfParent = mParent->GetParent();
            if (parentOfParent)
            {
                uint32 attributeIndex = parentOfParent->FindAttributeIndexByValuePointer(mParent);
                MCORE_ASSERT(attributeIndex != MCORE_INVALIDINDEX32);
                return parentOfParent->GetChildAttributeSettings(attributeIndex);
            }
        }

        return nullptr;
    }


    // scale the attributes
    void AttributeSettings::Scale(float scaleFactor)
    {
        if (mDefaultValue)
        {
            mDefaultValue->Scale(scaleFactor);
        }
        /*
            if (mMinValue)
                mMinValue->Scale( scaleFactor );

            if (mMaxValue)
                mMaxValue->Scale( scaleFactor );*/
    }


    /*
    // build a hierarchical string like "materialList.myMaterial.specularColor.rgb"
    void AttributeSettings::BuildHierarchicalName(String& outString)
    {
        outString.Clear();

        //String temp;

        // work our way up towards the root
        AttributeSettings* curSettings = this;
        while (curSettings)
        {
            AttributeSettings* nextSettings = curSettings->FindParentSettings();

            const bool hasName = (curSettings->GetInternalNameString().GetLength() > 0);

            outString.Insert(0, curSettings->GetInternalName());

            if (nextSettings && hasName)
                outString.Insert(0, ".");

            curSettings = nextSettings;
        }
    }
    */
}   // namespace MCore
