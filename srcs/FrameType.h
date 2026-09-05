/*
	mspot - an M17 hot-spot using an  M17 CC1200 Raspberry Pi Hat
				Copyright (C) 2026 Thomas A. Early

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <cstring>
#include <cstdint>

enum class EPayloadType { dataonly, c2_3200, c2_1600, packet };
enum class EEncryptType { none, scram8, scram16, scram24, aes128, aes192, aes256 };
enum class EMetaDatType { none, gnss, ecd, text, aes };

class CFrameType
{
public:
	CFrameType() : m_isSigned(false), m_can(0), m_payload(EPayloadType::packet), m_encrypt(EEncryptType::none), m_metatype(EMetaDatType::none) {}
	CFrameType(uint16_t t) { SetFrameType(t); }
	virtual ~CFrameType() {}
	void SetFrameType(uint16_t t);
	uint16_t GetFrameType();
	EPayloadType GetPayloadType()  const { return m_payload;  }
	EEncryptType GetEncryptType()  const { return m_encrypt;  }
	EMetaDatType GetMetaDataType() const { return m_metatype; }
	bool         GetIsSigned()     const { return m_isSigned; }
	uint8_t      GetCan()          const { return m_can; }

	void SetPayloadType(EPayloadType t)  { m_payload = t;         }
	void SetEncryptType(EEncryptType t)  { m_encrypt = t;         }
	void SetMetaDataType(EMetaDatType t) { m_metatype = t;        }
	void SetSigned(bool issigned)        { m_isSigned = issigned; }
	void SetCan(uint8_t can)             { m_can = can;           }

private:
	bool m_isSigned;
	uint16_t m_can;
	EPayloadType m_payload;
	EEncryptType m_encrypt;
	EMetaDatType m_metatype;
};
