#ifndef _CGAME_DESIGN_H_
#define _CGAME_DESIGN_H_

#include "../../DataType/CType.h"

class DesignClassDataInt32
{
public:
	DesignClassDataInt32(){};

	int _beginByte;
	int *_data;
	int _totalAtribute, _totalData;

	void Load();
	void UnLoad();

	int getValue(int row, int atribute);
};

class DesignClassDataInt64
{
public:
	DesignClassDataInt64(){};

	int _beginByte;
	INT64 *_data;
	int _totalAtribute, _totalData;

	void Load();
	void UnLoad();

	INT64 getValue(int row, int atribute);
};

class DesignClassDataFloat
{
public:
	DesignClassDataFloat(){};

	int _beginByte;
	float *_data;
	int _totalAtribute, _totalData;

	void Load();
	void UnLoad();

	float getValue(int row, int atribute);
};

class DesignClassDataString
{
public:
	DesignClassDataString(){};

	int _beginByte;
	char **_data;
	int _totalAtribute, _totalData;

	void Load();
	void UnLoad();

	char *getValue(int row, int atribute);
};

class CGameDesign
{
public:
	CGameDesign(){};

	class FileLevel
	{
		public:
		FileLevel()
		{

		};

		void Load()
		{
			SheetGeneral.Load();
			Sheetitem.Load();
			Sheetcharacter.Load();
			Sheetlevel.Load();

		};

		void UnLoad()
		{
			SheetGeneral.UnLoad();
			Sheetitem.UnLoad();
			Sheetcharacter.UnLoad();
			Sheetlevel.UnLoad();

		};

		class CSheetGeneral
		{
			public:
			DesignClassDataFloat floatValue;
			DesignClassDataString stringValue;

			CSheetGeneral()
			{
				floatValue._beginByte = 0;
				stringValue._beginByte = 24;

			};
			enum enumFloat
			{
				paramater = 0,

			};
			enum enumString
			{
				type = 0,

			};


			void Load()
			{
				floatValue.Load();
				stringValue.Load();

			};

			void UnLoad()
			{
				floatValue.UnLoad();
				stringValue.UnLoad();

			};
		};
		class CSheetitem
		{
			public:
			DesignClassDataFloat floatValue;

			CSheetitem()
			{
				floatValue._beginByte = 108;

			};
			enum enumFloat
			{
				score_min = 0,
				score_max = 1,
				speed_decrease = 2,

			};


			void Load()
			{
				floatValue.Load();

			};

			void UnLoad()
			{
				floatValue.UnLoad();

			};
		};
		class CSheetcharacter
		{
			public:
			DesignClassDataString stringValue;

			CSheetcharacter()
			{
				stringValue._beginByte = 140;

			};
			enum enumString
			{
				name = 0,

			};


			void Load()
			{
				stringValue.Load();

			};

			void UnLoad()
			{
				stringValue.UnLoad();

			};
		};
		class CSheetlevel
		{
			public:
			DesignClassDataInt32 int32Value;
			DesignClassDataFloat floatValue;

			CSheetlevel()
			{
				int32Value._beginByte = 273;
				floatValue._beginByte = 2053;

			};
			enum enumInt32
			{
				bounce_count = 0,

			};
			enum enumFloat
			{
				speed_increase = 0,

			};


			void Load()
			{
				int32Value.Load();
				floatValue.Load();

			};

			void UnLoad()
			{
				int32Value.UnLoad();
				floatValue.UnLoad();

			};
		};

		CSheetGeneral SheetGeneral;
		CSheetitem Sheetitem;
		CSheetcharacter Sheetcharacter;
		CSheetlevel Sheetlevel;

	};
	class FileAchievement
	{
		public:
		FileAchievement()
		{

		};

		void Load()
		{
			SheetObjective.Load();
			SheetMultiplier.Load();
			SheetSheet3.Load();

		};

		void UnLoad()
		{
			SheetObjective.UnLoad();
			SheetMultiplier.UnLoad();
			SheetSheet3.UnLoad();

		};

		class CSheetObjective
		{
			public:
			DesignClassDataInt32 int32Value;
			DesignClassDataString stringValue;

			CSheetObjective()
			{
				int32Value._beginByte = 3833;
				stringValue._beginByte = 3941;

			};
			enum enumInt32
			{
				value = 0,

			};
			enum enumString
			{
				name = 0,
				Description = 1,
				GC_ID = 2,
				GGPLAY_ID = 3,

			};


			void Load()
			{
				int32Value.Load();
				stringValue.Load();

			};

			void UnLoad()
			{
				int32Value.UnLoad();
				stringValue.UnLoad();

			};
		};
		class CSheetMultiplier
		{
			public:
			DesignClassDataInt32 int32Value;
			DesignClassDataFloat floatValue;

			CSheetMultiplier()
			{
				int32Value._beginByte = 5627;
				floatValue._beginByte = 5835;

			};
			enum enumInt32
			{
				level = 0,
				multi = 1,

			};
			enum enumFloat
			{
				speed_title = 0,
				speed_change = 1,

			};


			void Load()
			{
				int32Value.Load();
				floatValue.Load();

			};

			void UnLoad()
			{
				int32Value.UnLoad();
				floatValue.UnLoad();

			};
		};
		class CSheetSheet3
		{
			public:

			CSheetSheet3()
			{

			};


			void Load()
			{

			};

			void UnLoad()
			{

			};
		};

		CSheetObjective SheetObjective;
		CSheetMultiplier SheetMultiplier;
		CSheetSheet3 SheetSheet3;

	};

	FileLevel Level;
	FileAchievement Achievement;

};

#endif