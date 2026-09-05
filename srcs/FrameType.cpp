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

#include <cassert>
#include "FrameType.h"

void CFrameType::SetFrameType(uint16_t t)
{
	// 1 Get the payload type
	m_encrypt = EEncryptType::none;
	m_metatype = EMetaDatType::none;
	if (t & 1u)
	{
		switch ((t >> 1) & 0x3u)
		{
		case 1u:
			m_payload = EPayloadType::dataonly;
			break;
		default:
		case 2u:
			m_payload = EPayloadType::c2_3200;
			break;
		case 3u:
			m_payload = EPayloadType::c2_1600;
			break;
		}
	} else {
		m_payload = EPayloadType::packet;
	}
	uint8_t subtype = (t >> 5) & 0x3u;	// get the subtype field
	switch ((t >> 3) & 0x3u) // get the encrypt field
	{
	case 0u:	// no encryption
		switch (subtype)
		{
		default:
		case 0u:
			m_metatype = EMetaDatType::text;
			break;
		case 1u:
			m_metatype = EMetaDatType::gnss;
			break;
		case 2u:
			m_metatype = EMetaDatType::ecd;
			break;
		}
		break;
	case 1u: // scrambler
		switch (subtype)
		{
		default:
		case 0u:
			m_encrypt = EEncryptType::scram8;
			break;
		case 1u:
			m_encrypt = EEncryptType::scram16;
			break;
		case 2u:
			m_encrypt = EEncryptType::scram24;
			break;
		}
		break;
	case 2u: // aes
		switch (subtype)
		{
		default:
		case 0u:
			m_encrypt = EEncryptType::aes128;
			break;
		case 1u:
			m_encrypt = EEncryptType::aes192;
			break;
		case 2u:
			m_encrypt = EEncryptType::aes256;
			break;
		}
		break;
	}
	m_isSigned = (t & 0x800u) ? true : false;
	m_can = (t >> 7) & 0xfu;
}

uint16_t CFrameType::GetFrameType()
{
	uint16_t ft;
	// payload type
	switch (m_payload)
	{
	case EPayloadType::packet:
		ft = 0u;
		break;
	case EPayloadType::dataonly:
		ft = 3u;
		break;
	case EPayloadType::c2_3200:
		ft = 5u;
		break;
	case EPayloadType::c2_1600:
		ft = 7u;
		break;
	}

	switch (m_encrypt)
	{
	case EEncryptType::none:
		break;
	case EEncryptType::scram8:
		ft |= 0x8u;
		break;
	case EEncryptType::scram16:
		ft |= 0x28u;
		break;
	case EEncryptType::scram24:
		ft |= 0x48u;
		break;
	case EEncryptType::aes128:
		ft |= 0x18u;
		break;
	case EEncryptType::aes192:
		ft |= 0x38u;
		break;
	case EEncryptType::aes256:
		ft |= 0x58u;
		break;
	}

	switch (m_metatype)
	{
	default:
		break;
	case EMetaDatType::gnss:
		ft |= 0x20;
		break;
	case EMetaDatType::ecd:
		ft |= 0x40u;
	}

	ft |= (m_can << 7);

	if (m_isSigned)
		ft |= 0x800u;
	return ft;
}
