using Datamodel;

namespace MeshCullerForSource2;

internal class Program
{	
	private static readonly string consolePrefix = "[Mesh Culler Source 2]";
	private static readonly string selectionSetPrefix = "MeshCuller_";

	/**
	 * Loop over all top level selection sets
	 *  Find any selection set with the prefix "MeshCuller_" anything after this will be refered to as "ClusterName"
	 * 
	 *   Place the generated mesh under models/MeshCuller/{mapPath}/{clusterName}.vmdl
	 * 
	 * Set "transformLocked" on the generated entity
	 */

	static void Main(string[] args) {
		Console.WriteLine($"{consolePrefix} - Build {GetBuildNo()}");

		var testMapPath = "C:\\Users\\jonas\\Downloads\\untitled_1.vmap";
		//var testOutMapPath = "C:\\Users\\jonas\\Downloads\\untitled_2.vmap";

		var map = Datamodel.Datamodel.Load(testMapPath);

		var rootSelectionSet = DMParser<CMapSelectionSet>.ParseElement((Element)map.Root["rootSelectionSet"]);
		var visibility = DMParser<CVisibilityMgr>.ParseElement((Element)map.Root["visbility"]);

		foreach (var child in rootSelectionSet.Children) {
			if (child.SelectionSetName.StartsWith(selectionSetPrefix)) {
				//child.Element
				//rootSelectionSet.Element
				
				//map.Root.Remove(child.Element);
			}
			//Console.WriteLine();
		}

		//map.Save("D:\\SteamLibrary\\steamapps\\common\\Counter-Strike Global Offensive\\content\\csgo_addons\\warden\\maps\\prefabs\\de_warden\\a_long_t_ramp.vmap", map.Encoding, map.EncodingVersion);

		Console.WriteLine($"{consolePrefix} Reloading Hammer ...");
		if (ProcessHandler.FocusWindow("cs2", "Hammer")) {
			SendKeys.SendWait("^(+{F12})"); // <ctrl>+<shift>+<F12> HammerApp: FileReload
			SendKeys.SendWait("{LEFT}");
			SendKeys.SendWait("{ENTER}");
		} else {
			Console.WriteLine($"{consolePrefix} Unable to reload hammer, hammer is most likely not running.");
		}

		Console.WriteLine($"{consolePrefix} Completed!");
	}
	
	public static string GetBuildNo() {
		return "0.0.1";
	}
}
