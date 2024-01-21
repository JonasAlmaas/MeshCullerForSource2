using Datamodel.Format;
using DMElement = Datamodel.Element;
using DMElementArray = Datamodel.ElementArray;

namespace MeshCullerForSource2;

public class BaseDMElement
{
	public DMElement Element { get; set; } = null!;
	[DMProperty(name: "id")]
	public Guid Id { get; set; }
}

public class DMParser<T>
	where T : BaseDMElement, new()
{
	public static T ParseElement(DMElement e) {
		var instance = new T();

		var props = instance.GetType().GetProperties();
		foreach (var prop in props) {
			if (prop.PropertyType.FullName == "Datamodel.Element") {
				instance.Element = e;
				continue;
			}

			var attrib = prop.GetCustomAttributes(typeof(DMProperty), false).FirstOrDefault() as DMProperty;

			var name = attrib?.Name ?? prop.Name;
			//var optional = attrib?.Optional ?? false;

			if (name == "id") {
				instance.Id = e.ID;
				continue;
			}

			var element = e[name];

			if (element is DMElement element1) {
				var parseElementMethod = typeof(DMParser<>)
					.MakeGenericType(prop.PropertyType)
					.GetMethod("ParseElement")!;

				var value = parseElementMethod.Invoke(null, [element1]);

				prop.SetValue(instance, value);
			} else if (element is DMElementArray elementArray) {
				var arrayElementsType = prop.PropertyType.GetElementType();
				if (arrayElementsType != null) {
					var arrayElements = Array.CreateInstance(arrayElementsType, elementArray.Count);

					for (var i = 0; i < elementArray.Count; i++) {
						var arrayElement = elementArray[i];

						if (arrayElementsType.FullName == "Datamodel.Element") {
							arrayElements.SetValue(arrayElement, i);
						} else {
							var parseElementMethod = typeof(DMParser<>)
								.MakeGenericType(arrayElementsType)
								.GetMethod("ParseElement")!;

							var value = parseElementMethod.Invoke(null, [arrayElement]);

							arrayElements.SetValue(value, i);
						}
					}

					prop.SetValue(instance, arrayElements);
				}
			} else {
				prop.SetValue(instance, element);
			}
		}

		return instance;
	}
}
