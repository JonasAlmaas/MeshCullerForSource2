using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

namespace MeshCullerForSource2;

internal class ProcessHandler
{
	public static bool FocusWindow(string processName, string titlePrefix) {
		var processes = Process.GetProcessesByName(processName);

		foreach (Process process in processes) {
			var hWnd = IntPtr.Zero;
			foreach (ProcessThread thread in process.Threads) {
				hWnd = findWindowByTitle(thread.Id, titlePrefix);
				if (hWnd != IntPtr.Zero) {
					break;
				}
			}

			if (hWnd != IntPtr.Zero) {
				SetForegroundWindow(hWnd);
				return true;
			}
		}

		return false;
	}

	private static IntPtr findWindowByTitle(int threadId, string titlePrefix) {
		var hWnd = IntPtr.Zero;
		do {
			hWnd = FindWindowEx(IntPtr.Zero, hWnd, null, null);
			int length = GetWindowTextLength(hWnd);
			if (length > 0) {
				var builder = new StringBuilder(length + 1);
				GetWindowText(hWnd, builder, builder.Capacity);
				string title = builder.ToString();
				if (title.StartsWith(titlePrefix, StringComparison.OrdinalIgnoreCase)) {
					uint windowThreadId = GetWindowThreadProcessId(hWnd, out _);
					if (windowThreadId == threadId)
						return hWnd;
				}
			}
		} while (hWnd != IntPtr.Zero);

		return IntPtr.Zero;
	}

	[DllImport("user32.dll")]
	[return: MarshalAs(UnmanagedType.Bool)]
	private static extern bool SetForegroundWindow(IntPtr hWnd);

	[DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
	private static extern int GetWindowTextLength(IntPtr hWnd);

	[DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
	private static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);

	[DllImport("user32.dll", SetLastError = true)]
	private static extern IntPtr FindWindowEx(IntPtr hwndParent, IntPtr hwndChildAfter, string? lpszClass, string? lpszWindow);

	[DllImport("user32.dll", SetLastError = true)]
	private static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint lpdwProcessId);
}
