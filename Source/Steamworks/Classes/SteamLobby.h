//Copyright 2016 davevillz, https://github.com/davevill



#pragma once

#include "GameFramework/Info.h"
#include "Steamworks.h"
#include "SteamLobby.generated.h"






UENUM(BlueprintType)
enum ESteamLobbyType
{
	Private,
	FriendsOnly,
	Public,
	Invisible
};



USTRUCT(BlueprintType)
struct FSteamLobbyDataEntry
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FString Key;

	UPROPERTY(BlueprintReadWrite)
	FString Value;
};

USTRUCT(BlueprintType)
struct FSteamLobbyInfo
{
	GENERATED_USTRUCT_BODY()

public:

	CSteamID Id;

	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	bool bFriend;

	UPROPERTY(BlueprintReadOnly)
	TArray<FSteamLobbyDataEntry> Metadata;

	bool bDataRequested;

	FSteamLobbyInfo()
	{
		bDataRequested = false;
		bFriend = false;
	}


	void UpdateData(bool bPassive = false);

};

USTRUCT(BlueprintType)
struct FSteamLobbyMember
{
	GENERATED_USTRUCT_BODY()

public:


	CSteamID UserId;

	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	int32 Index;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* AvatarTexture;

};


USTRUCT()
struct FSteamLobbyVoiceBuffer
{
	GENERATED_USTRUCT_BODY()

public:

	CSteamID UserId;

	UPROPERTY(BlueprintReadOnly)
	UAudioComponent* AudioComponent;

	UPROPERTY()
	float TalkTimer;

};




UCLASS()
class STEAMWORKS_API USteamLobby : public UObject
{
	GENERATED_BODY()



	float UpdateTimer;


	bool bVoiceChatEnabled;


	TArray<uint8> CompressedVoiceBuffer;
	TArray<uint8> RawVoiceBuffer;


	UAudioComponent* CreateVoiceAudioComponent() const;

	UPROPERTY()
	TArray<FSteamLobbyVoiceBuffer> VoiceBuffers;


	FSteamLobbyVoiceBuffer& GetVoiceBuffer(CSteamID Id);

	UPROPERTY()
	float LocalUserTalkTimer;


protected:

	CSteamID LocalUserId;

	int32 MemberLimit;

	bool bLocalUserOwner;

public:


	TWeakObjectPtr<class USteamworksManager> Manager;


	UPROPERTY(BlueprintReadOnly, Category="Steam Lobby")
	TArray<FSteamLobbyMember> Members;

	UPROPERTY(BlueprintReadOnly, Category="Steam Lobby")
	FSteamLobbyInfo Info;


	UPROPERTY(BlueprintReadWrite, Category="Steam Lobby")
	float VoiceVolume;

	/** Disables voice chat and other Tick related task that could slow down the gameplay
	  * Usually this should be enabled when the lobby transitions to playing mode */
	UPROPERTY(BlueprintReadWrite, Category="Steam Lobby")
	bool bLightModeEnabled;


	USteamLobby();

	/** Called internally to initialize the lobby inner workings */
	virtual void Initialize();

	virtual void Shutdown();


	virtual void Tick(float DeltaTime);


	/** Returns true if the local user is the owner */
	UFUNCTION(BlueprintPure, Category="Steam Lobby")
	bool IsLocalUserOwner() const;

	/** Returns the number of users curently in the lobby */
	UFUNCTION(BlueprintPure, Category="Steam Lobby")
	int32 GetNumLobbyMembers() const;

	/** Returns the maximum lobby members, 0 if unlimited */
	UFUNCTION(BlueprintPure, Category="Steam Lobby")
	int32 GetLobbyMemberLimit() const;

	/** Sets the lobby member limit */
	UFUNCTION(BlueprintCallable, Category="Steam Lobby")
	void SetLobbyMemberLimit(int32 MemberLimit);

	/** Sets lobby data, should update all members as well */
	UFUNCTION(BlueprintCallable, Category="Steam Lobby")
	void SetLobbyData(const FString& Key, const FString& Value);

	/** Retrieves lobby data by key */
	UFUNCTION(BlueprintPure, Category="Steam Lobby")
	FString GetLobbyData(const FString& Key) const;

	/** Retrieves per-member metadata */
	UFUNCTION(BlueprintPure, Category="Steam Lobby")
	FString GetMemberData(int32 Index, const FString& Key) const;

	/** Sets per-member metadata for the local user */
	UFUNCTION(BlueprintCallable, Category="Steam Lobby")
	void SetMemberData(const FString& Key, const FString& Value);


	/** True if the given member index is talking */
	UFUNCTION(BlueprintPure, Category="Steam Lobby")
	bool IsMemberTalking(int32 Index) const;

	/** Enables P2P voice chat */
	UFUNCTION(BlueprintCallable, Category="Steam Lobby")
	void SetVoiceChat(bool bEnabled);

	/** True if voice is enabled */
	UFUNCTION(BlueprintPure, Category="Steam Lobby")
	bool IsVoiceChatEnabled() const { return bVoiceChatEnabled; }


	/** Opens the steam invite friend overlay */
	UFUNCTION(BlueprintCallable, Category="Steam Lobby")
	void InviteFriends();

	/** Sets the lobby type, must be lobby owner */
	UFUNCTION(BlueprintCallable, Category="Steam Lobby")
	void SetLobbyType(ESteamLobbyType Type);


	void UpdateMemberList();


	virtual void OnLobbyDataUpdated();
	virtual void OnLobbyChatMsg(CSteamID Sender, const TArray<char, TInlineAllocator<256>>& Message) {};

	virtual void OnLightTick() {};


	const FSteamLobbyMember* GetMemberByIndex(int32 Index) const;


	int32 GetMemberIndexById(CSteamID Id) const;
	
	FSteamLobbyMember* GetMutableMemberById(CSteamID Id);



	virtual UWorld* GetWorld() const override;

};
