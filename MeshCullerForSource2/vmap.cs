using Datamodel.Format;
using DMElement = Datamodel.Element;

namespace MeshCullerForSource2;

public class CMapMesh : ElementBase
{
	[DMProperty(name: "referenceID")]
	public ulong ReferenceId { get; set; }
}

public class EditGameClassProps : ElementBase
{
	[DMProperty(name: "classname")]
	public string ClassName {get;set;}=null!;
	[DMProperty(name: "model")]
	public string Model {get;set;}=null!;
	[DMProperty(name: "rendercolor")]
	public string RenderColor {get;set;}=null!;
	[DMProperty(name: "skin")]
	public string Skin {get;set;}=null!;
	[DMProperty(name: "solid")]
	public string Solid {get;set;}=null!;
	[DMProperty(name: "materialoverride")]
	public string MaterialOverride {get;set;}=null!;
}

public class CMapEntity : ElementBase
{
	[DMProperty(name: "nodeID")]
	public int NodeId { get; set; }
	[DMProperty(name: "referenceID")]
	public ulong ReferenceId { get; set; }
	[DMProperty(name: "entity_properties")]
	public EditGameClassProps EntityProperties { get; set; } = null!;
}

public class CObjectSelectionSetDataElement : ElementBase
{
	[DMProperty(name: "selectedObjects")]
	public DMElement SelectedObjects { get; set; } = null!;
}

public class CMapSelectionSet : ElementBase
{
	[DMProperty(name: "selectionSetName")]
	public string SelectionSetName { get; set; } = null!;
	public CObjectSelectionSetDataElement selectionSetData { get; set;}=null!;
	[DMProperty(name: "children")]
	public CMapSelectionSet[] Children { get; set; } = null!;
}
