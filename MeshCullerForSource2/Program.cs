using System.Numerics;

using Datamodel;

namespace MeshCullerForSource2;

internal class MeshCullerProp
{
	public string ModelPath = null!;
	public Vector3 Origin;
	public Vector3 Angles;
	public Vector3 Scale;
}

internal class MeshCullerVolume
{
	// TODO
}

internal class MeshCullerCluster(string name)
{
	public readonly string Name = name;
	
	public List<MeshCullerProp> Props = [];
	//public List<MeshCullerVolume> Volumes = [];
}

internal class Program
{	
	private static readonly string consolePrefix = "[Mesh Culler Source 2]";
	private static readonly string selectionSetPrefix = "MeshCuller_";

	static void Main(string[] args) {
		Console.WriteLine($"{consolePrefix} - Build {GetBuildNo()}");

		List<MeshCullerCluster> clusters = [];

		var testMapPath = "D:\\SteamLibrary\\steamapps\\common\\Counter-Strike Global Offensive\\content\\csgo_addons\\warden\\maps\\prefabs\\de_warden\\a_long_t_ramp.vmap";
		//var testMapPath = "C:\\Users\\jonas\\Downloads\\untitled_1.vmap";
		//var testOutMapPath = "C:\\Users\\jonas\\Downloads\\untitled_2.vmap";

		using var file = File.Open(testMapPath, FileMode.Open);
		
		var map = Datamodel.Datamodel.Load(file);
		
		var rootSelectionSet = DMParser<CMapSelectionSet>.ParseElement((Element)map.Root["rootSelectionSet"]);
		var visibility = DMParser<CVisibilityMgr>.ParseElement((Element)map.Root["visbility"]);

		foreach (var selectionSet in rootSelectionSet.Children) {
			if (selectionSet.SelectionSetName.StartsWith(selectionSetPrefix)) {
				var cluster = new MeshCullerCluster(selectionSet.SelectionSetName[selectionSetPrefix.Length..]);

				var selectedObjects = selectionSet.SelectionSetData.SelectedObjects;
				foreach (var obj in selectedObjects) {
					// TODO:
					//	Hide this entity
					//	Mark as editor only?
					
					/*for (var i = 0; i < visibility.Nodes.Length; ++i) {
						if (visibility.Nodes[i] == obj) {
							visibility.HiddenFlags[i] |= 1;
							break;
						}
					}*/
					
					if (DMUtils.IsCMapEntity(obj)) {
						var entity = DMParser<CMapEntity>.ParseElement(obj);
						
						if (entity.EntityProperties.ClassName != "prop_static") {
							Console.WriteLine($"{consolePrefix} Entity class \"{entity.EntityProperties.ClassName}\" is not supported. Only prop_static's are currently supported!");
						} else {
							cluster.Props.Add(new MeshCullerProp {
								ModelPath = entity.EntityProperties.Model,
								Origin = entity.Origin,
								Angles = entity.Angles,
								Scale = entity.Scale
							});
						}
					} else if (DMUtils.IsCMapMesh(obj)) {
						var mesh = DMParser<CMapMesh>.ParseElement(obj);
						// TODO
					}
				}

				clusters.Add(cluster);
			}
		}

		/**
		 * Generate new culled meshes
		 * Add them into the map
		 * Set "transformLocked" on the new meshes
		 * Make sure the older meshes are hidden. Maybe marked as editor only?
		 */

		//map.Save(file, "keyvalues2", 4); // This currently doesn't work with cs2 ...

		Console.WriteLine($"{consolePrefix} Reloading Hammer ...");
		if (hammerReloadFile()) {
			Console.WriteLine($"{consolePrefix} Completed!");
		} else {
			Console.WriteLine($"{consolePrefix} Unable to reload hammer, hammer is most likely not running.");
		}
	}
	
	public static string GetBuildNo() {
		return "0.0.1";
	}

	private static bool hammerReloadFile() {
		if (ProcessHandler.FocusWindow("cs2", "Hammer")) {
			SendKeys.SendWait("^(+{F12})"); // <ctrl>+<shift>+<F12> HammerApp: FileReload
			SendKeys.SendWait("{LEFT}");
			SendKeys.SendWait("{ENTER}");
			return true;
		} else {
			return false;
		}
	}
}
