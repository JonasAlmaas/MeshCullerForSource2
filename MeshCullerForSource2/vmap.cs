﻿using System.Numerics;

using Datamodel;
using Datamodel.Format;
using DMElement = Datamodel.Element;

namespace MeshCullerForSource2;

public class DMUtils
{
	public static bool IsCMapEntity(DMElement e) => e.ContainsKey("entity_properties");
	public static bool IsCMapMesh(DMElement e) => e.ContainsKey("meshData");
}

public class MapNode : BaseDMElement
{
	[DMProperty(name: "nodeID")]
	public int NodeId { get; set; }
	[DMProperty(name: "referenceID")]
	public ulong ReferenceId { get; set; }

	[DMProperty(name: "origin")]
	public Vector3 Origin { get; set; }
	[DMProperty(name: "angles")]
	public QAngle Angles { get; set; }
	[DMProperty(name: "scales")]
	public Vector3 Scale { get; set; } = new Vector3(1, 1, 1);

	[DMProperty(name: "editorOnly")]
	public bool EditorOnly { get; set; }
}

public class CMapMesh : MapNode
{
}

public class EditGameClassProps : BaseDMElement
{
	[DMProperty(name: "classname")]
	public string ClassName { get; set; } = null!;
	[DMProperty(name: "model")]
	public string Model { get; set; } = null!;
	[DMProperty(name: "rendercolor")]
	public string RenderColor { get; set; } = null!;
	[DMProperty(name: "skin")]
	public string Skin { get; set; } = null!;
	[DMProperty(name: "solid")]
	public string Solid { get; set; } = null!;
	[DMProperty(name: "materialoverride")]
	public string MaterialOverride { get; set; } = null!;
}

public class CMapEntity : MapNode
{
	[DMProperty(name: "entity_properties")]
	public EditGameClassProps EntityProperties { get; set; } = null!;	
}

public class CObjectSelectionSetDataElement : BaseDMElement
{
	[DMProperty(name: "selectedObjects")]
	public DMElement[] SelectedObjects { get; set; } = null!;
}

public class CMapSelectionSet : BaseDMElement
{
	[DMProperty(name: "selectionSetName")]
	public string SelectionSetName { get; set; } = null!;
	[DMProperty(name: "selectionSetData")]
	public CObjectSelectionSetDataElement SelectionSetData { get; set;} = null!;
	[DMProperty(name: "children")]
	public CMapSelectionSet[] Children { get; set; } = null!;
}

public class CVisibilityMgr : BaseDMElement
{
	[DMProperty(name: "nodes")]
	public DMElement[] Nodes { get; set; } = null!;
	[DMProperty(name: "hiddenFlags")]
	public Datamodel.IntArray HiddenFlags { get; set; } = null!;
}
