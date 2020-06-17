// ObjectMessaging - Tom Shinton 2020

#pragma once

#include <Runtime/CoreUObject/Public/UObject/GCObject.h>
#include <Runtime/Core/Public/Templates/SharedPointer.h>

//-----------------------------------------------------------------------------------------------

DEFINE_LOG_CATEGORY_STATIC(ObjectMessagingListenerLog, Log, Log)

//-----------------------------------------------------------------------------------------------

class FBindingBase
{
public:

	virtual bool ShouldExecute(const UScriptStruct& InStructType) = 0;
	virtual void Execute(const void* InData) = 0;
	virtual ~FBindingBase() {};
};

//-----------------------------------------------------------------------------------------------

template<typename TEvent>
class FBinding : public FBindingBase
{
public:

	FBinding(const TFunction<void(const TEvent&)>& InCallback)
		: Callback(InCallback)
		, BindingType(TEvent::StaticStruct())
	{};

	~FBinding()
	{

	};

	bool ShouldExecute(const UScriptStruct& InStructType) override
	{
		return BindingType == &InStructType;
	}

	void Execute(const void* InData) override
	{
		if (Callback != nullptr && InData != nullptr)
		{
			if (const TEvent* TypedData = (TEvent*)InData)
			{
				Callback(*TypedData);
			}
		}
	}

private:

	TFunction<void(const TEvent&)> Callback;
	const UScriptStruct* BindingType;
};

//-----------------------------------------------------------------------------------------------

class FObjectMessagingListener : public FGCObject
{
public:

	FObjectMessagingListener()
	{};

	~FObjectMessagingListener()
	{
		BindingMap.Reset();
	};

	template<typename TEvent>
	void Bind(const TFunction<void(const TEvent&)>& InCallback)
	{
		BindingMap.FindOrAdd(TEvent::StaticStruct()).Add(MakeShareable(new FBinding<TEvent>(InCallback)));
	}

	template<typename TEvent>
	void InvokeNewEvent(const TEvent& InEvent)
	{
		UScriptStruct* IncomingStaticStruct = TEvent::StaticStruct();
		if (BindingMap.Contains(IncomingStaticStruct))
		{
			for (TSharedPtr<FBindingBase> Binding : BindingMap[IncomingStaticStruct])
			{
				if (Binding.IsValid())
				{
					if (const void* DataRaw = &InEvent)
					{
						Binding->Execute(DataRaw);
					}
				}
			}
		}
	}

	void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		Collector.AddReferencedObjects(BindingMap);
	}

private:

	TMap<UScriptStruct*, TArray<TSharedPtr<FBindingBase>>> BindingMap;
};

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------