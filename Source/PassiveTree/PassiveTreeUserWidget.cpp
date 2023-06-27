// Fill out your copyright notice in the Description page of Project Settings.


#include "PassiveTreeUserWidget.h"
#include "PassiveTreeGameModeBase.h"
#include "FPassiveTreeState.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"

void UPassiveTreeUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!RootCanvasPanel)
	{
		// 루트 패널
		RootCanvasPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanelRoot")));
	}
	if (!ButtonReset)
	{
		// 초기화 버튼
		ButtonReset = Cast<UButton>(GetWidgetFromName(TEXT("ButtonReset")));
	}
	if (ButtonReset)
	{
		// 초기화 버튼 이벤트
		ButtonReset->OnClicked.AddUniqueDynamic(this, &UPassiveTreeUserWidget::OnResetPressed);
	}
	if (!TextBlockTotalCost)
	{
		// 총 투자 코스트 텍스트
		TextBlockTotalCost = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlockTotalCost")));
		RefreshTotalCostText(0);
	}
}

void UPassiveTreeUserWidget::InitTreeComponent()
{
	if (!RootCanvasPanel) return;

	APassiveTreeGameModeBase* PassiveTreeGameMode = Cast<APassiveTreeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!PassiveTreeGameMode) return;

	const FPassiveTree& passiveTree = PassiveTreeGameMode->GetPassiveTree();
	FPassiveTreeState& passiveTreeState = PassiveTreeGameMode->GetPassiveTreeState();

	const TMap<PNodeID, FPassiveNodeData>& nodes = passiveTree.GetNodes();
	const TMap<PNodeID, TSet<PNodeID>>& links = passiveTree.GetLinks();

	// 패시브트리 링크 내용에 따라 링크 Widget 추가
	for (auto& linkElem : links)
	{
		PNodeID linkIDFrom = linkElem.Key;
		const TSet<PNodeID>& linkIDToSet = linkElem.Value;

		for (PNodeID linkIDTo : linkIDToSet)
		{
			if (linkIDFrom < linkIDTo)
			{
				FString EdgeName = FString::Printf(TEXT("Edge_%d_%d"), linkIDFrom, linkIDTo);

				UPassiveTreeEdgeImage* NewEdge = WidgetTree->ConstructWidget<UPassiveTreeEdgeImage>(UPassiveTreeEdgeImage::StaticClass(), *EdgeName);
				RootCanvasPanel->AddChildToCanvas(NewEdge);

				FIntPoint linkID(linkIDFrom, linkIDTo);

				const FPassiveNodeData* nodeFrom = nodes.Find(linkIDFrom);
				const FPassiveNodeData* nodeTo = nodes.Find(linkIDTo);

				FVector2D posFrom(nodeFrom->pos);
				FVector2D posTo(nodeTo->pos);

				NewEdge->InitSetting(linkID, posFrom, posTo);
				NewEdge->SetEdgeColor(UPassiveTreeEdgeImage::Color_Deselect);

				EdgeUIMap.Add(linkID, NewEdge);
			}
		}
	}

	// 패시브트리 링크 내용에 따라 노드 Widget 추가
	for (auto& nodeElem : nodes)
	{
		PNodeID nodeID = nodeElem.Key;
		const FPassiveNodeData& nodeData = nodeElem.Value;

		FString NodeName = FString::Printf(TEXT("Node_%d"), nodeID);
		FString NodeTextName = NodeName + TEXT("_TEXT");

		UPassiveTreeNodeButton* NewButton = WidgetTree->ConstructWidget<UPassiveTreeNodeButton>(UPassiveTreeNodeButton::StaticClass(), *NodeName);
		RootCanvasPanel->AddChildToCanvas(NewButton);

		UTextBlock* NewTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), *NodeTextName);

		FVector2D widgetPos(nodeData.pos);

		bool isRootNode = nodeID == FPassiveTree::RootNodeID;

		FString text;
		if (isRootNode)
		{
			text = TEXT("S");
		}
		else
		{
			text = FString::Printf(TEXT("%d"), nodeData.cost);
		}

		NewButton->InitSetting(nodeID, widgetPos, NewTextBlock, text, this);
		NodeUIMap.Add(nodeID, NewButton);

		if (isRootNode)
		{
			NewButton->SetNodeColor(UPassiveTreeNodeButton::Color_Root);
		}
		else
		{
			NewButton->SetNodeColor(UPassiveTreeNodeButton::Color_Deselect);
		}
	}

	RefreshTotalCostText(passiveTreeState.GetTotalUsedCost());
}

void UPassiveTreeUserWidget::OnNodePressed(uint32 NodeID)
{
	APassiveTreeGameModeBase* PassiveTreeGameMode = Cast<APassiveTreeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!PassiveTreeGameMode) return;

	const FPassiveTree& passiveTree = PassiveTreeGameMode->GetPassiveTree();
	FPassiveTreeState& passiveTreeState = PassiveTreeGameMode->GetPassiveTreeState();


	// 선택한 노드까지 경로 탐색 및 투자 처리
	TArray<PNodeID> path;
	passiveTreeState.FindPath(path, NodeID);
	passiveTreeState.AssignPath(path);

	// 새로 선택한 노드 경로들의 노드 및 엣지 표시 갱신
	for (int32 i = 0; i < path.Num(); ++i)
	{
		PNodeID pathNodeID = path[i];
		if (pathNodeID != FPassiveTree::RootNodeID)
		{
			auto& uiNode = NodeUIMap[pathNodeID];
			uiNode->SetNodeColor(UPassiveTreeNodeButton::Color_Select);
		}

		if (i != 0)
		{
			PNodeID otherNodeID = path[i - 1];
			FIntPoint linkID;
			if (pathNodeID < otherNodeID)
			{
				linkID.X = pathNodeID;
				linkID.Y = otherNodeID;
			}
			else
			{
				linkID.X = otherNodeID;
				linkID.Y = pathNodeID;
			}

			auto& edgeNode = EdgeUIMap[linkID];
			edgeNode->SetEdgeColor(UPassiveTreeEdgeImage::Color_Select);
		}
	}

	RefreshTotalCostText(passiveTreeState.GetTotalUsedCost());
}

void UPassiveTreeUserWidget::OnResetPressed()
{
	APassiveTreeGameModeBase* PassiveTreeGameMode = Cast<APassiveTreeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!PassiveTreeGameMode) return;

	// 투자 상태 초기화
	FPassiveTreeState& passiveTreeState = PassiveTreeGameMode->GetPassiveTreeState();
	passiveTreeState.Reset();

	// 노드 표시상태 초기화
	for (auto& nodeInfo : NodeUIMap)
	{
		PNodeID nodeID = nodeInfo.Key;
		if (nodeID != FPassiveTree::RootNodeID)
		{
			auto& uiNode = nodeInfo.Value;
			uiNode->SetNodeColor(UPassiveTreeNodeButton::Color_Deselect);
		}
	}

	// 엣지 표시상태 초기화
	for (auto& edgeInfo : EdgeUIMap)
	{
		edgeInfo.Value->SetEdgeColor(UPassiveTreeEdgeImage::Color_Deselect);
	}

	// 총 투자 코스트 표시 갱신
	RefreshTotalCostText(passiveTreeState.GetTotalUsedCost());
}

void UPassiveTreeUserWidget::RefreshTotalCostText(uint32 totalCost)
{
	if (TextBlockTotalCost)
	{
		// 총 투자 코스트 갱신
		FText text = FText::Format(FText::FromString("Total Cost : {0}"), totalCost);
		TextBlockTotalCost->SetText(text);
	}
}
