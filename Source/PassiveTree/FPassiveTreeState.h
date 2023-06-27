// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPassiveTree.h"

/**
 * 패시브 트리의 유저 상태. 투자한 노드 정보 및 총 투자코스트 정보
 */
class FPassiveTreeState
{
public:
	FPassiveTreeState();
	~FPassiveTreeState();

	/* 패시브트리 연동 */
	void SetTree(FPassiveTree* _ptrTree);
	/* 투자노드 초기화 */
	void Reset();
	/* 이미 투자한 노드에서 선택한 노드까지 가장 짧은 경로 탐색 */
	void FindPath(TArray<PNodeID>& outPathReversed, PNodeID targetNodeID);
	/* 찾은 경로에 따라 투자 적용 */
	void AssignPath(const TArray<PNodeID>& path);
	/* 총 투자 비용 */
	uint32 GetTotalUsedCost();

	const FPassiveTree* GetTree() const;

protected:
	/* 연동된 패시브트리 */
	FPassiveTree* ptrTree;
	/* 투자한 노드들의 PNodeID */
	TSet<PNodeID> assignedNodeSet;
	/* 총 투자 비용 */
	uint32 totalUsedCost;
};
