/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * textblock.h: 
 *
 * Contact:
 *   Moonlight List (moonlight-list@lists.ximian.com)
 *
 * Copyright 2007 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 */

#ifndef __TEXTBLOCK_H__
#define __TEXTBLOCK_H__

#include <glib.h>
#include <cairo.h>

#include "frameworkelement.h"
#include "downloader.h"
#include "moon-path.h"
#include "thickness.h"
#include "fontfamily.h"
#include "fontstretch.h"
#include "fontstyle.h"
#include "fontweight.h"
#include "fontsource.h"
#include "layout.h"
#include "brush.h"
#include "fonts.h"
#include "control.h"

#define TEXTBLOCK_FONT_FAMILY  "Portable User Interface"
#define TEXTBLOCK_FONT_STRETCH FontStretchesNormal
#define TEXTBLOCK_FONT_WEIGHT  FontWeightsNormal
#define TEXTBLOCK_FONT_STYLE   FontStylesNormal

/* @Namespace=System.Windows.Documents */
class TextElement : public DependencyObject {
protected:
	virtual ~TextElement () {}

public:
	/* @GeneratePInvoke,GenerateCBinding,ManagedAccess=Protected */
	TextElement () {};

	/* @PropertyType=FontFamily,DefaultValue=FontFamily(TEXTBLOCK_FONT_FAMILY),GenerateAccessors */
	const static int FontFamilyProperty;
	/* @PropertyType=double,AutoCreator=CreateDefaultFontSize,GenerateAccessors */
	const static int FontSizeProperty;
	/* @PropertyType=FontStretch,DefaultValue=FontStretch(TEXTBLOCK_FONT_STRETCH),GenerateAccessors */
	const static int FontStretchProperty;
	/* @PropertyType=FontStyle,DefaultValue=FontStyle(TEXTBLOCK_FONT_STYLE),GenerateAccessors */
	const static int FontStyleProperty;
	/* @PropertyType=FontWeight,DefaultValue=FontWeight(TEXTBLOCK_FONT_WEIGHT),GenerateAccessors */
	const static int FontWeightProperty;
	/* @PropertyType=Brush,DefaultValue=new SolidColorBrush("black"),GenerateAccessors */
	const static int ForegroundProperty;
	/* @PropertyType=string,DefaultValue=\"en-US\",ManagedPropertyType=XmlLanguage,Validator=NonNullExceptionValidator,GenerateAccessors */
	const static int LanguageProperty;
	/* @PropertyType=TextDecorations,DefaultValue=TextDecorationsNone,ManagedPropertyType=TextDecorationCollection,GenerateAccessors */
	const static int TextDecorationsProperty;

	void SetFontFamily (FontFamily *family);
	FontFamily *GetFontFamily ();
	
	void SetFontSize (double size);
	double GetFontSize ();
	
	void SetFontStretch (FontStretch *stretch);
	FontStretch *GetFontStretch ();
	
	void SetFontStyle (FontStyle *style);
	FontStyle *GetFontStyle ();
	
	void SetFontWeight (FontWeight *weight);
	FontWeight *GetFontWeight ();
	
	void SetForeground (Brush *fg);
	Brush *GetForeground ();
	
	void SetLanguage (const char *language);
	const char *GetLanguage ();
	
	void SetTextDecorations (TextDecorations decorations);
	TextDecorations GetTextDecorations ();
};

/* @Namespace=System.Windows.Documents */
class Inline : public TextElement, public ITextAttributes {
	TextFontDescription *font;
	GPtrArray *downloaders;
	bool autogen;
	
	void AddFontResource (const char *resource);
	void AddFontSource (Downloader *downloader);
	
	void CleanupDownloaders ();
	
	void DownloaderComplete (Downloader *downloader);
	
	static void downloader_complete (EventObject *sender, EventArgs *calldata, gpointer closure);
	
 protected:
	virtual ~Inline ();
	
 public:
 	/* @PropertyType=FontFamily,DefaultValue=FontFamily(TEXTBLOCK_FONT_FAMILY),GenerateAccessors */
	const static int FontFamilyProperty;
 	/* @PropertyType=double,AutoCreator=CreateDefaultFontSize,GenerateAccessors */
	const static int FontSizeProperty;
 	/* @PropertyType=FontStretch,DefaultValue=FontStretch(TEXTBLOCK_FONT_STRETCH),GenerateAccessors */
	const static int FontStretchProperty;
 	/* @PropertyType=FontStyle,DefaultValue=FontStyle(TEXTBLOCK_FONT_STYLE),GenerateAccessors */
	const static int FontStyleProperty;
 	/* @PropertyType=FontWeight,DefaultValue=FontWeight(TEXTBLOCK_FONT_WEIGHT),GenerateAccessors */
	const static int FontWeightProperty;
 	/* @PropertyType=Brush,DefaultValue=new SolidColorBrush("black"),GenerateAccessors */
	const static int ForegroundProperty;
 	/* @PropertyType=TextDecorations,DefaultValue=TextDecorationsNone,ManagedPropertyType=TextDecorationCollection,GenerateAccessors */
	const static int TextDecorationsProperty;
	/* @PropertyType=string,DefaultValue=\"en-US\",Version=2.0,ManagedPropertyType=XmlLanguage,Validator=NonNullValidator,GenerateAccessors */
	const static int LanguageProperty;
	
	// internal properties to inherit the FontSource between inlines and textblocks
 	/* @PropertyType=FontSource,GenerateManagedDP=false,GenerateAccessors */
	const static int FontSourceProperty;
	
	/* @GenerateCBinding,GeneratePInvoke,ManagedAccess=Protected */
	Inline ();
	
	virtual void OnPropertyChanged (PropertyChangedEventArgs *args, MoonError *error);
	virtual void OnSubPropertyChanged (DependencyProperty *prop, DependencyObject *obj, PropertyChangedEventArgs *subobj_args);
	
	virtual bool PermitsMultipleParents () { return false; }
	
	//
	// Property Accessors
	//
	void SetFontFamily (FontFamily *value);
	FontFamily *GetFontFamily ();
	
	void SetLanguage (const char *lang);
	const char *GetLanguage ();
	
	void SetFontSize (double value);
	double GetFontSize ();
	
	void SetFontStretch (FontStretch *value);
	FontStretch *GetFontStretch ();
	
	void SetFontStyle (FontStyle *value);
	FontStyle *GetFontStyle ();
	
	void SetFontWeight (FontWeight *value);
	FontWeight *GetFontWeight ();
	
	void SetFontSource (FontSource *source);
	FontSource *GetFontSource ();
	
	void SetForeground (Brush *value);
	Brush *GetForeground ();
	
	void SetTextDecorations (TextDecorations decorations);
	TextDecorations GetTextDecorations ();
	
	//
	// Non-DependencyProperty Accessors
	//
	void SetAutogenerated (bool autogen) { this->autogen = autogen; }
	bool GetAutogenerated () { return autogen; }
	
	//
	// ITextAttributes Interface Methods
	//
	virtual TextDecorations Decorations () { return GetTextDecorations (); }
	virtual TextFontDescription *FontDescription () { return font; }
	virtual Brush *Foreground (bool selected) { return GetForeground (); }
	virtual Brush *Background (bool selected) { return NULL; }
	
	//
	// Convenience Methods
	//
	bool UpdateFontDescription (const char *source, bool force);
	
	virtual bool Equals (Inline *item);
};


/* @Namespace=System.Windows.Documents */
/* @ContentProperty=Child */
class InlineUIContainer : public Inline {

protected:
	virtual ~InlineUIContainer () {}

public:
	/* @GenerateCBinding,GeneratePInvoke */
	InlineUIContainer () {}

	/* @PropertyType=UIElement,GenerateAccessors */
	const static int ChildProperty;

	void SetChild (UIElement *child);
	UIElement *GetChild ();
};

/* @Namespace=System.Windows.Documents */
class LineBreak : public Inline {
 protected:
	virtual ~LineBreak () {}
	
 public:
	/* @GenerateCBinding,GeneratePInvoke */
	LineBreak () { SetObjectType (Type::LINEBREAK); }
};


/* @ContentProperty="Text" */
/* @Namespace=System.Windows.Documents */
class Run : public Inline {
 protected:
	virtual ~Run () {}
	
 public:
 	/* @PropertyType=string,ManagedFieldAccess=Internal,GenerateAccessors */
	const static int TextProperty;
	
	/* @GenerateCBinding,GeneratePInvoke */
	Run () { SetObjectType (Type::RUN); }
	
	virtual bool Equals (Inline *item);
	
	//
	// Property Accessors
	//
	void SetText (const char *text);
	const char *GetText ();
};


class TextBlockDynamicPropertyValueProvider;

/* @ContentProperty="Inlines" */
/* @Namespace=System.Windows.Controls */
class TextBlock : public FrameworkElement {
	friend class TextBlockDynamicPropertyValueProvider;
	
	TextFontDescription *font;
	GPtrArray *downloaders;
	Downloader *source;
	TextLayout *layout;
	char *font_source;
	
	double actual_height;
	double actual_width;
	
	int setvalue:1;
	int was_set:1;
	int dirty:1;
	
	void Layout (Size constraint);
	void Paint (cairo_t *cr);
	
	char *GetTextInternal (InlineCollection *inlines);
	void SetTextInternal (const char *text);
	
	void AddFontResource (const char *resource);
	void AddFontSource (Downloader *downloader);
	
	void UpdateLayoutAttributes ();
	bool UpdateFontDescription (bool force);
	bool UpdateFontDescriptions (bool force);
	
	void CleanupDownloaders (bool all);
	
	void DownloaderComplete (Downloader *downloader);
	
	static void downloader_complete (EventObject *sender, EventArgs *calldata, gpointer closure);
	
 protected:
	virtual ~TextBlock ();
	
 public:
	/* @PropertyType=FontFamily,DefaultValue=FontFamily(TEXTBLOCK_FONT_FAMILY),GenerateAccessors */
	const static int FontFamilyProperty;
	/* @PropertyType=double,AutoCreator=CreateDefaultFontSize,GenerateAccessors */
	const static int FontSizeProperty;
	/* @PropertyType=FontStretch,DefaultValue=FontStretch(TEXTBLOCK_FONT_STRETCH),GenerateAccessors */
	const static int FontStretchProperty;
	/* @PropertyType=FontStyle,DefaultValue=FontStyle(TEXTBLOCK_FONT_STYLE),GenerateAccessors */
	const static int FontStyleProperty;
	/* @PropertyType=FontWeight,DefaultValue=FontWeight(TEXTBLOCK_FONT_WEIGHT),GenerateAccessors */
	const static int FontWeightProperty;
	/* @PropertyType=FontSource,ManagedFieldAccess=Internal,Version=2.0,GenerateAccessors */
	const static int FontSourceProperty;
	/* @PropertyType=Brush,DefaultValue=new SolidColorBrush("black"),GenerateAccessors */
	const static int ForegroundProperty;
	/* @PropertyType=InlineCollection,AutoCreateValue,ManagedFieldAccess=Internal,ManagedSetterAccess=Internal,GenerateAccessors */
	const static int InlinesProperty;
	/* @PropertyType=string,DefaultValue=\"\",GenerateAccessors */
	const static int TextProperty;
	/* @PropertyType=double,DefaultValue=0.0,Version=2.0,GenerateAccessors */
	const static int LineHeightProperty;
	/* @PropertyType=LineStackingStrategy,DefaultValue=LineStackingStrategyMaxHeight,Version=2.0,GenerateAccessors */
	const static int LineStackingStrategyProperty;
	/* @PropertyType=Thickness,DefaultValue=Thickness (0),Version=2.0,GenerateAccessors */
	const static int PaddingProperty;
	/* @PropertyType=TextAlignment,DefaultValue=TextAlignmentLeft,Version=2.0,GenerateAccessors */
	const static int TextAlignmentProperty;
	/* @PropertyType=TextDecorations,DefaultValue=TextDecorationsNone,ManagedPropertyType=TextDecorationCollection,GenerateAccessors */
	const static int TextDecorationsProperty;
	/* @PropertyType=TextWrapping,DefaultValue=TextWrappingNoWrap,GenerateAccessors */
	const static int TextWrappingProperty;
	/* @PropertyType=TextTrimming */
	const static int TextTrimmingProperty;	
	
	/* @GenerateCBinding,GeneratePInvoke */
	TextBlock ();
	
	void SetFontSource (Downloader *downloader);
	
	//
	// Overrides
	//
	virtual void Render (cairo_t *cr, Region *region, bool path_only = false);
	virtual Size MeasureOverride (Size availableSize);
	virtual Size ArrangeOverride (Size finalSize);
	virtual Size ComputeActualSize ();
	virtual void ComputeBounds ();
	virtual Point GetTransformOrigin ();
	virtual void OnPropertyChanged (PropertyChangedEventArgs *args, MoonError *error);
	virtual void OnSubPropertyChanged (DependencyProperty *prop, DependencyObject *obj, PropertyChangedEventArgs *subobj_args);
	virtual void OnCollectionItemChanged (Collection *col, DependencyObject *obj, PropertyChangedEventArgs *args);
	virtual void OnCollectionChanged (Collection *col, CollectionChangedEventArgs *args);
	virtual bool CanFindElement () { return true; }
	virtual bool InsideObject (cairo_t *cr, double x, double y);
	
	void SetFontFamily (FontFamily *family);
	FontFamily *GetFontFamily ();
	
	void SetFontSize (double size);
	double GetFontSize ();
	
	void SetFontStretch (FontStretch *stretch);
	FontStretch *GetFontStretch ();
	
	void SetFontStyle (FontStyle *style);
	FontStyle *GetFontStyle ();
	
	void SetFontWeight (FontWeight *weight);
	FontWeight *GetFontWeight ();
	
	void SetFontSource (FontSource *source);
	FontSource *GetFontSource ();
	
	void SetForeground (Brush *fg);
	Brush *GetForeground ();
	
	void SetInlines (InlineCollection *inlines);
	InlineCollection *GetInlines ();
	
	void SetLineHeight (double height);
	double GetLineHeight ();
	
	void SetLineStackingStrategy (LineStackingStrategy strategy);
	LineStackingStrategy GetLineStackingStrategy ();
	
	void SetPadding (Thickness *padding);
	Thickness *GetPadding ();
	
	void SetText (const char *text);
	const char *GetText ();
	
	void SetTextAlignment (TextAlignment alignment);
	TextAlignment GetTextAlignment ();
	
	void SetTextDecorations (TextDecorations decorations);
	TextDecorations GetTextDecorations ();
	
	void SetTextWrapping (TextWrapping wrapping);
	TextWrapping GetTextWrapping ();
};

/* @Namespace=System.Windows.Documents */
class Block : public TextElement {
protected:
	virtual ~Block () {}

public:
	/* @GenerateCBinding,GeneratePInvoke */
	Block () {}

	/* @PropertyType=TextAlignment,DefaultValue=TextAlignmentLeft,GenerateAccessors */
	const static int TextAlignmentProperty;

	TextAlignment GetTextAlignment ();
	void SetTextAlignment (TextAlignment value);
};

/* @Namespace=System.Windows.Documents */
/* @ContentProperty=Inlines */
class Paragraph : public Block {
protected:
	virtual ~Paragraph () {}

public:
	/* @GeneratePInvoke,GenerateCBinding */
	Paragraph () {}

	/* @PropertyType=InlineCollection,GenerateAccessors,ManagedFieldAccess=Internal */
	const static int InlinesProperty;

	InlineCollection *GetInlines ();
	void SetInlines (InlineCollection *value);
};

/* @Namespace=System.Windows.Documents */
/* @ContentProperty=Inlines */
class Span : public Inline {
protected:
	virtual ~Span () {}

public:
	/* @GeneratePInvoke,GenerateCBinding */
	Span () {}

	/* @PropertyType=InlineCollection,GenerateAccessors,ManagedFieldAccess=Internal */
	const static int InlinesProperty;

	InlineCollection *GetInlines ();
	void SetInlines (InlineCollection *value);
};

/* @Namespace=System.Windows.Documents */
class Bold : public Span {
protected:
	virtual ~Bold () {}

public:
	/* @GeneratePInvoke,GenerateCBinding */
	Bold () {}
};

/* @Namespace=System.Windows.Documents */
class Italic : public Span {
protected:
	virtual ~Italic () {}

public:
	/* @GeneratePInvoke,GenerateCBinding */
	Italic () {}
};

/* @Namespace=System.Windows.Documents */
class Underline : public Span {
protected:
	virtual ~Underline () {}

public:
	/* @GeneratePInvoke,GenerateCBinding */
	Underline () {}
};

/* @Namespace=System.Windows.Documents */
class Hyperlink : public Span {
protected:
	virtual ~Hyperlink () {}

public:
	/* @GenerateCBinding,GeneratePInvoke */
	Hyperlink () {}

	/* @PropertyType=object */
	const static int CommandParameterProperty;
	/* @PropertyType=object,ManagedPropertyType=ICommand */
	const static int CommandProperty;
	/* @PropertyType=Brush */
	const static int MouseOverForegroundProperty;
	/* @PropertyType=TextDecorationCollection */
	const static int MouseOverTextDecorationsProperty;
	/* @PropertyType=Uri */
	const static int NavigateUriProperty;
	/* @PropertyType=char* */
	const static int TargetNameProperty;
};

/* @Namespace=System.Windows.Controls */
/* @ContentProperty=Blocks */
class RichTextArea : public Control {
protected:
	virtual ~RichTextArea () {}

public:
	/* @GeneratePInvoke,GenerateCBinding */
	RichTextArea () {}

	/* @PropertyType=bool,DefaultValue=false,GenerateAccessors */
	const static int IsReadOnlyProperty;

	/* @PropertyType=TextWrapping,DefaultValue=TextWrappingNoWrap,GenerateAccessors */
	const static int TextWrappingProperty;

	/* @PropertyType=Collection,ManagedPropertyType=BlockCollection,AutoCreateValue,ManagedSetterAccess=Private,ManagedFieldAccess=Private */
	const static int BlocksProperty;

	bool GetIsReadOnly ();
	void SetIsReadOnly (bool value);

	TextWrapping GetTextWrapping ();
	void SetTextWrapping (TextWrapping value);

	/* @DelegateType=RoutedEventHandler */
	const static int SelectionChangedEvent;
};

#endif /* __TEXTBLOCK_H__ */
