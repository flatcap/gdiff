#ifndef _DERIVED_H_
#define _DERIVED_H_

#include <gtk/gtk.h>

#define GTK_TYPE_RICHARD		(gtk_richard_get_type ())
#define GTK_RICHARD(obj)		GTK_CHECK_CAST (obj, gtk_richard_get_type (), GtkRichard)
#define GTK_RICHARD_CLASS(klass)	GTK_CHECK_CLASS_CAST (klass, gtk_richard_get_type (), GtkRichardClass)
#define GTK_IS_RICHARD(obj)		GTK_CHECK_TYPE (obj, gtk_richard_get_type ())
	
typedef struct _GtkRichard       GtkRichard;
typedef struct _GtkRichardClass  GtkRichardClass;

struct _GtkRichard
{
	GtkWidget widget;

	int value;
};

struct _GtkRichardClass
{
	GtkWidgetClass parent_class;
};

guint      gtk_richard_get_type (void);
GtkWidget* gtk_richard_new (void);

#endif // _DERIVED_H_

