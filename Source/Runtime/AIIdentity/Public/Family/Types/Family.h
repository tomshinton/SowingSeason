#pragma once

struct FFamily
{
	FFamily(const FGuid& InFoundingMember)
		: FamilyID(FGuid::NewGuid())
		, Members()
	{
		Members.Add(InFoundingMember);
	};

public:

	void AddMemberToFamily(const FGuid& InNewMember)
	{
		const int32 NewMemberIndex = Members.AddUnique(InNewMember);	
		check(NewMemberIndex != INDEX_NONE);
	}

	FGuid GetID() const 
	{
		return FamilyID;
	}

	uint8 Count() const 
	{
		return Members.Num();
	}

private:

	FGuid FamilyID;
	TArray<FGuid> Members;
};