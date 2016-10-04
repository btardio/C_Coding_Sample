/*
 *    Resume Vector
 *    Copyright (C) 2016 Brandon C Tardio
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *    
 *   Contact: BTardio@gmail.com
 *            818 424 6838
 *
 */

#ifndef __CDDS_DIALOG_NEW_H
#define __CDDS_DIALOG_NEW_H

//#include <gtk/gtk.h>
//#include "../cdds_window.h"

#include "../cdds.h"

G_BEGIN_DECLS

#define CDS_AREA_TYPE_DIALOG (cds_area_dialog_get_type ())

G_DECLARE_FINAL_TYPE(CdsAreaDialog, cds_area_dialog, CDS_AREA, DIALOG, GtkDialog)

GType                   cds_area_dialog_get_type             (void);
CdsAreaDialog          *cds_area_dialog_new                  ( CdsAreaWindow* win );
void                    cds_area_dialog_destroy              (CdsAreaDialog*);
void                    cds_area_dialog_activate_callback    (GtkMenuItem *, gpointer );

G_END_DECLS

struct _CdsAreaDialog
{
  GtkDialog parent_instance;
  
  GtkEntry* entry_width;
  GtkEntry* entry_height;
  
};

struct _CdsAreaDialogClass
{
  GtkDialogClass parent_class_instance;
};




#endif // __CDDS_DIALOG_NEW_H

