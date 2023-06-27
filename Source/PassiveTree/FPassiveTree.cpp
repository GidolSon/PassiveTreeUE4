// Fill out your copyright notice in the Description page of Project Settings.


#include "FPassiveTree.h"

FPassiveTree::FPassiveTree()
{
}

FPassiveTree::~FPassiveTree()
{
}

const TMap<PNodeID, FPassiveNodeData>& FPassiveTree::GetNodes() const
{
	return nodes;
}
const TMap<PNodeID, TSet<PNodeID>>& FPassiveTree::GetLinks() const
{
	return links;
}


void FPassiveTree::_clear()
{
	nodes.Empty();
	links.Empty();
}

void FPassiveTree::_addNode(const FPassiveNodeData& data)
{
	nodes.Emplace(data.id, data);
}

void FPassiveTree::_addLink(const PNodeID id1, const PNodeID id2)
{
	if (id1 != id2)
	{
		__addLink_SingleDirection(id1, id2);
		__addLink_SingleDirection(id2, id1);
	}
}

void FPassiveTree::__addLink_SingleDirection(const PNodeID id1, const PNodeID id2)
{
	TSet<PNodeID>* iter = links.Find(id1);
	if (iter == nullptr)
	{
		iter = &links.Emplace(id1, TSet<PNodeID>());
	}
	if (iter != nullptr)
	{
		iter->Add(id2);
	}
}
