//
// Contact:
//   Moonlight List (moonlight-list@lists.ximian.com)
//
// Copyright 2009 Novell, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls.Primitives;

using Hyena.Collections;

namespace System.Windows.Controls {

	public sealed class ItemContainerGenerator : IRecyclingItemContainerGenerator, IItemContainerGenerator {

		public event ItemsChangedEventHandler ItemsChanged;

		Dictionary <int, DependencyObject> IndexToContainer {
			get; set;
		}

		Queue <DependencyObject> Cache {
			get; set;
		}

		internal GenerationState GenerationState {
			get; set;
		}

		ItemsControl Owner {
			get; set;
		}

		Panel Panel {
			get { return Owner.Panel; }
		}
		
		RangeCollection RealizedElements {
			get; set;
		}

		internal ItemContainerGenerator (ItemsControl owner)
		{
			Cache = new Queue <DependencyObject> ();
			IndexToContainer = new Dictionary <int, DependencyObject> ();
			Owner = owner;
			RealizedElements = new RangeCollection ();
		}

		public DependencyObject ContainerFromIndex (int index)
		{
			DependencyObject container;
			IndexToContainer.TryGetValue (index, out container);
			return container;
		}

		public DependencyObject ContainerFromItem (object item)
		{
			if (item == null)
				return null;
			int index = Owner.Items.IndexOf (item);
			return Owner.GetContainerItem (Owner.Items.IndexOf (item));
		}

		public GeneratorPosition GeneratorPositionFromIndex (int itemIndex)
		{
			if (RealizedElements.Contains (itemIndex))
				return new GeneratorPosition (RealizedElements.IndexOf (itemIndex), 0);
			if (RealizedElements.Count == 0)
				return new GeneratorPosition (-1, itemIndex + 1);

			int index = -1;
			for (int i = 0; i < RealizedElements.Count; i ++) {
				if (RealizedElements [i] > itemIndex)
					break;
				index = i;
			}
			if (index == -1) {
				return new GeneratorPosition (index, itemIndex + 1);
			} else {
				return new GeneratorPosition (index, itemIndex - RealizedElements [index]);
			}
		}

		public int IndexFromContainer (DependencyObject container)
		{
			int count = Owner.Items.Count;
			for (int i = 0; i < count; i++)
				if (Owner.GetContainerItem (i) == container)
					return i;
			return -1;
		}

		public int IndexFromGeneratorPosition (GeneratorPosition position)
		{
			// We either have everything realised or nothing realised, so we can
			// simply just add Index and Offset together to get the right index (i think)
			if (position.Index == -1) {
				if (position.Offset < 0)
					return Panel.Children.Count + position.Offset;
				//else if (position.Offset == 0)
				//	return 0;
				else
					return position.Offset - 1;
			} else {
				if (position.Index > Panel.Children.Count)
					return -1;
				if (position.Index > 0 && position.Index < RealizedElements.Count)
					return RealizedElements [position.Index] + position.Offset;
				return position.Index + position.Offset;
			}
		}

		public object ItemFromContainer (DependencyObject container)
		{
			int count = Owner.Items.Count;
			for (int i = 0; i < count; i ++)
				if (Owner.GetContainerItem (i) == container)
					return Owner.Items [i];
			return null;
		}

		internal DependencyObject Realize (int index)
		{
			//RealizedElements.Add (index);
			return Owner.GetContainerForItem ();
			IItemContainerGenerator g = (IItemContainerGenerator) this;
			bool realized;
			using (var v = g.StartAt (g.GeneratorPositionFromIndex (index), GeneratorDirection.Forward, true))
				return g.GenerateNext (out realized);
		}

		DependencyObject IItemContainerGenerator.GenerateNext (out bool isNewlyRealized)
		{
			int index;
			// This is relative to the realised elements.
			int startAt = GenerationState.Position.Index;
			if (startAt == -1) {
				if (GenerationState.Position.Offset < 0)
					index = Panel.Children.Count + GenerationState.Position.Offset;
				else if (GenerationState.Position.Offset == 0)
					index = 0;
				else
					index = GenerationState.Position.Offset - 1;
			} else if (startAt >= 0 && startAt < RealizedElements.Count) {
				// We're starting relative to an already realised element
				index = RealizedElements [startAt] + GenerationState.Position.Offset;
			} else {
				index = -1;
			}

			if (index < 0 || index >= Panel.Children.Count) {
				isNewlyRealized = false;
				return null;
			}

			RealizedElements.Add (index);
			isNewlyRealized = Cache.Count > 0;
			DependencyObject container = isNewlyRealized ? Cache.Dequeue () : Owner.GetContainerForItem ();
			IndexToContainer [index] = container;
			return container;
		}

		ItemContainerGenerator IItemContainerGenerator.GetItemContainerGeneratorForPanel (Panel panel)
		{
			// FIXME: Double check this, but i think it's right
			return panel == Panel ? this : null;
		}

		void IItemContainerGenerator.PrepareItemContainer (DependencyObject container)
		{
			// FIXME: Does this do anything?
		}

		void IItemContainerGenerator.Remove (GeneratorPosition position, int count)
		{
			int index = IndexFromGeneratorPosition (position);
			for (int i = 0; i < count; i++)
				RealizedElements.Remove (index + i);
		}

		void IItemContainerGenerator.RemoveAll ()
		{
			RealizedElements.Clear ();
		}

		IDisposable IItemContainerGenerator.StartAt (GeneratorPosition position,
							     GeneratorDirection direction,
							     bool allowStartAtRealizedItem)
		{
			if (GenerationState != null)
				throw new InvalidOperationException ("Cannot call StartAt while a generation operation is in progress");

			GenerationState = new GenerationState {
				AllowStartAtRealizedItem = allowStartAtRealizedItem,
				Direction = direction,
				Position = position,
				Generator = this
			};
			return GenerationState;
		}

		void IRecyclingItemContainerGenerator.Recycle (GeneratorPosition position,
							       int count)
		{
			throw new NotImplementedException ();
		}
	}
}

