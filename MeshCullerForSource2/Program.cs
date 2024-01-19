using Datamodel;

namespace MeshCullerForSource2;

internal class Program
{
	static void Main(string[] args) {
		Console.WriteLine($"Mesh Culler Source 2 - Build {GetBuildNo()}");

		var testMapPath = "C:\\Users\\jonas\\Downloads\\untitled_1.vmap";
		//var testOutMapPath = "C:\\Users\\jonas\\Downloads\\untitled_2.vmap";

		var map = Datamodel.Datamodel.Load(testMapPath);

		var rootSelectionSet = DMParser<CMapSelectionSet>.ParseElement((Element)map.Root["rootSelectionSet"]);

		Console.WriteLine("Mesh Culler Source 2 - Completed!");
	}
	
	public static string GetBuildNo() {
		return "0.0.1";
	}
}
