using Datamodel;

namespace MeshCullerForSource2;

internal class Program
{
	static void Main(string[] args) {
		Console.WriteLine($"Mesh Culler Source 2 - Build {GetBuildNo()}");

		Console.WriteLine("Mesh Culler Source 2 - Completed!");
	}
	
	public static string GetBuildNo() {
		return "0.0.1";
	}
}
