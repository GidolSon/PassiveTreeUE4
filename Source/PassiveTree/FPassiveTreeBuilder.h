// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPassiveTree.h"

/**
 * 
 */
class FPassiveTreeBuilder
{
	/* 패시브트리 랜덤생성시 셀 점유 체크 */
	class CellChecker
	{
	public:
		CellChecker(int32 cellCountX, int32 cellCountY);
		/* 특정 노드가 차지하는 공간 체크. 비어있을시 점유하고 노드 자신의 PNodeID 리턴, 이미 다른 노드가 점유시 기존 점유노드 PNodeID 리턴 */
		PNodeID TryOccupy(const FPassiveNodeData& nodeData, float nodeHalfSize);
	private:
		/* 특정 공간 크기로 분할된 Cell 정보. 점유한 노드의 PNodeID 저장 */
		TArray<TArray<PNodeID>> cellUsed;
		/* 총 셀의 개수 */
		FIntPoint cellCount;
	};

	/* 객체 생성 방지 처리 */
	virtual void VirtualFuncForPreventInstantiate() = 0;

public:
	/* 패시브트리 랜덤 생성 로직 */
	static void MakeRandomPassiveTree(FPassiveTree& tree);
};
