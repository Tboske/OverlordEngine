#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pBinReader = new BinaryReader();
	pBinReader->Open(loadInfo.assetFullPath);

	if (!pBinReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}
	
	//See BMFont Documentation for Binary Layout

	//Parse the Identification bytes (B,M,F)
	const byte b = pBinReader->Read<byte>();
	const byte m = pBinReader->Read<byte>();
	const byte f = pBinReader->Read<byte>();
	const byte version = pBinReader->Read<byte>();
	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	if (b != 'B' || m != 'M' || f != 'F')
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font");
		return nullptr;
	}

	//Parse the version (version 3 required)
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	if (version < 3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;
	}

	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve the FontSize [fontDesc.fontSize]
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	//Retrieve the FontName [fontDesc.fontName]
	byte blockID = pBinReader->Read<byte>();
	int blockSize = pBinReader->Read<int>();

	fontDesc.fontSize = pBinReader->Read<short>();

	pBinReader->MoveBufferPosition(12);
	fontDesc.fontName = pBinReader->ReadNullString();

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
	//Retrieve PageCount
	//> if pagecount > 1
	//	> Log Error (Only one texture per font is allowed!)
	//Advance to Block2 (Move Reader)
	blockID = pBinReader->Read<byte>();
	blockSize = pBinReader->Read<int>();

	pBinReader->MoveBufferPosition(4);

	fontDesc.textureWidth = pBinReader->Read<short>();
	fontDesc.textureHeight = pBinReader->Read<short>();

	if (pBinReader->Read<short>() > 1)
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed");

	pBinReader->MoveBufferPosition(5);

	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve the PageName (BinaryReader::ReadNullString)
	//Construct the full path to the page texture file
	//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
	//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
	//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]
	blockID = pBinReader->Read<byte>();
	blockSize = pBinReader->Read<int>();

	const std::wstring pageName = pBinReader->ReadNullString();
	if (pageName.empty())
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty]");

	std::wstring path = loadInfo.assetFullPath.parent_path().wstring();
	path += L"\\";
	path += pageName;
	fontDesc.pTexture = ContentManager::Load<TextureData>(path);


	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve Character Count (see documentation)
	//Create loop for Character Count, and:
	//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
	//> Create instance of FontMetric (struct)
	//	> Set Character (CharacterId) [FontMetric::character]
	//	> Retrieve Xposition (store Local)
	//	> Retrieve Yposition (store Local)
	//	> Retrieve & Set Width [FontMetric::width]
	//	> Retrieve & Set Height [FontMetric::height]
	//	> Retrieve & Set OffsetX [FontMetric::offsetX]
	//	> Retrieve & Set OffsetY [FontMetric::offsetY]
	//	> Retrieve & Set AdvanceX [FontMetric::advanceX]
	//	> Retrieve & Set Page [FontMetric::page]
	//	> Retrieve Channel (BITFIELD!!!) 
	//		> See documentation for BitField meaning [FontMetrix::channel]
	//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
	//
	//> Insert new FontMetric to the metrics [font.metrics] map
	//	> key = (wchar_t) charId
	//	> value = new FontMetric
	//(loop restarts till all metrics are parsed)

	blockID = pBinReader->Read<byte>();
	blockID;
	blockSize = pBinReader->Read<int>();

	const auto charcount{ blockSize / 20 };
	for (int i = 0; i < charcount; ++i)
	{
		const auto id = static_cast<wchar_t>(pBinReader->Read<int>());
		//if (!pSpriteFont->IsCharValid(id))
		//{
		//	Logger::LogWarning(L"character is not valid");
		//	continue;
		//}
		FontMetric& metric = fontDesc.metrics[id];
		metric.character = id;


		metric.texCoord.x = static_cast<float>(pBinReader->Read<short>()) / fontDesc.textureWidth;
		metric.texCoord.y = static_cast<float>(pBinReader->Read<short>()) / fontDesc.textureHeight;

		metric.width = pBinReader->Read<short>();
		metric.height = pBinReader->Read<short>();

		metric.offsetX = pBinReader->Read<short>();
		metric.offsetY = pBinReader->Read<short>();

		metric.advanceX = pBinReader->Read<short>();

		metric.page = pBinReader->Read<byte>();

		const byte channelBitmask = pBinReader->Read<byte>();
		if (channelBitmask & 1)
			metric.channel = 2; //B
		else if (channelBitmask & 2)
			metric.channel = 1; //G
		else if (channelBitmask & 4)
			metric.channel = 0; //R
		else if (channelBitmask & 8)
			metric.channel = 3; //A
		else if (channelBitmask & 16)
			metric.channel = 4; //RGBA
	}

	//Done!
	delete pBinReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
