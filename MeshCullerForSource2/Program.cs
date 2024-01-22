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

		var mapPath = "D:\\SteamLibrary\\steamapps\\common\\Counter-Strike Global Offensive\\content\\csgo_addons\\warden\\maps\\prefabs\\de_warden\\a_long_t_ramp.vmap";

		var dmx = Datamodel.Datamodel.Load(mapPath);

		var rootSelectionSet = DMParser<CMapSelectionSet>.ParseElement((Element)dmx.Root["rootSelectionSet"]);
		var visibility = DMParser<CVisibilityMgr>.ParseElement((Element)dmx.Root["visbility"]);

		foreach (var selectionSet in rootSelectionSet.Children) {
			if (selectionSet.SelectionSetName.StartsWith(selectionSetPrefix)) {
				var cluster = new MeshCullerCluster(selectionSet.SelectionSetName[selectionSetPrefix.Length..]);

				var selectedObjects = selectionSet.SelectionSetData.SelectedObjects;
				foreach (var obj in selectedObjects) {
					for (var i = 0; i < visibility.Nodes.Length; ++i) {
						if (visibility.Nodes[i] == obj) {
							visibility.HiddenFlags[i] |= 1;
							break;
						}
					}

					if (DMUtils.IsCMapEntity(obj)) {
						var entity = DMParser<CMapEntity>.ParseElement(obj);
						entity.Element["editorOnly"] = true;

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
					}
					else if (DMUtils.IsCMapMesh(obj)) {
						var mesh = DMParser<CMapMesh>.ParseElement(obj);
						mesh.Element["editorOnly"] = true;
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

		using (var memoryStream = new MemoryStream()) {
			dmx.Save(memoryStream, "keyvalues2", 4);
			dmx.Dispose();

			var bytes = memoryStream.ToArray();
			memoryStream.Read(bytes, 0, (int)memoryStream.Length);

			using (var file = new FileStream(mapPath, FileMode.Open, FileAccess.Write)) {
				file.Write(bytes, 0, bytes.Length);
			}
		}

		Thread.Sleep(100);

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
